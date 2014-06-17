#include "scripting.hpp"
#include "../level/level.hpp"
#include "../level/level_player.hpp"
#include "../core/sprite_manager.hpp"
#include "../core/property_helper.hpp"
#include "../core/filesystem/resource_manager.hpp"

#include "objects/mrb_smc.hpp"
#include "objects/mrb_eventable.hpp"
#include "objects/mrb_uids.hpp"
#include "objects/sprites/mrb_sprite.hpp"
#include "objects/sprites/mrb_moving_sprite.hpp"
#include "objects/sprites/mrb_animated_sprite.hpp"
#include "objects/sprites/mrb_particle_emitter.hpp"
#include "objects/misc/mrb_level.hpp"
#include "objects/misc/mrb_level_player.hpp"
#include "objects/misc/mrb_audio.hpp"
#include "objects/misc/mrb_input.hpp"
#include "objects/misc/mrb_timer.hpp"
#include "objects/enemies/mrb_enemy.hpp"
#include "objects/enemies/mrb_beetle.hpp"
#include "objects/enemies/mrb_beetle_barrage.hpp"
#include "objects/enemies/mrb_eato.hpp"
#include "objects/enemies/mrb_flyon.hpp"
#include "objects/enemies/mrb_furball.hpp"
#include "objects/enemies/mrb_gee.hpp"
#include "objects/enemies/mrb_krush.hpp"
#include "objects/enemies/mrb_rokko.hpp"
#include "objects/enemies/mrb_spika.hpp"
#include "objects/enemies/mrb_spikeball.hpp"
#include "objects/enemies/mrb_static.hpp"
#include "objects/enemies/mrb_thromp.hpp"
#include "objects/enemies/mrb_turtle.hpp"
#include "objects/enemies/mrb_turtle_boss.hpp"
#include "objects/enemies/mrb_pip.hpp"
#include "objects/enemies/mrb_larry.hpp"
#include "objects/powerups/mrb_powerup.hpp"
#include "objects/powerups/mrb_mushroom.hpp"
#include "objects/powerups/mrb_fireplant.hpp"
#include "objects/powerups/mrb_moon.hpp"
#include "objects/powerups/mrb_star.hpp"
#include "objects/boxes/mrb_box.hpp"
#include "objects/boxes/mrb_spinbox.hpp"
#include "objects/boxes/mrb_textbox.hpp"
#include "objects/boxes/mrb_bonusbox.hpp"
#include "objects/specials/mrb_level_exit.hpp"
#include "objects/specials/mrb_level_entry.hpp"
#include "objects/specials/mrb_path.hpp"
#include "objects/specials/mrb_lava.hpp"
#include "objects/specials/mrb_enemy_stopper.hpp"
#include "objects/specials/mrb_goldpiece.hpp"
#include "objects/specials/mrb_jumping_goldpiece.hpp"
#include "objects/specials/mrb_crate.hpp"

////////////////////////////////////////
// Be sure to review docs/scripting.md!
////////////////////////////////////////

// Extern
mrb_data_type SMC::Scripting::rtSMC_Scriptable = {"SmcScriptable", NULL};

namespace SMC
{

	namespace Scripting
	{

		cMRuby_Interpreter::cMRuby_Interpreter(cLevel* p_level)
		{
			// Set member variables
			mp_level = p_level;
			mp_mruby = mrb_open();

			// Load our extensions into mruby
			Load_Scripts();
		}

		cMRuby_Interpreter::~cMRuby_Interpreter()
		{
			// Get all the registered timers from mruby
			mrb_value klass = mrb_obj_value(mrb_class_get(mp_mruby, "Timer"));
			mrb_value rb_timers = mrb_iv_get(mp_mruby, klass, mrb_intern_cstr(mp_mruby, "instances"));

			// Stop ’em all! (and free them)
			while(true) {
				// Retrieve timer
				mrb_value rb_timer = mrb_ary_shift(mp_mruby, rb_timers);
				if (mrb_nil_p(rb_timer))
					break;

				// Free C++ part. The mruby part is out of scope now (shifted from
				// the instance array) and will be GC’ed (would anyway due to termination
				// further below). Note cTimer’s destructor calls Interrupt() on the timer.
				cTimer* p_timer = Get_Data_Ptr<cTimer>(mp_mruby, rb_timer);
				delete p_timer;
			}

			// Terminate mruby interpreter
			mrb_close(mp_mruby);
		}

		mrb_state* cMRuby_Interpreter::Get_MRuby_State()
		{
			return mp_mruby;
		}

		cLevel* cMRuby_Interpreter::Get_Level()
		{
			return mp_level;
		}

		mrb_value cMRuby_Interpreter::Run_Code_In_Context(const std::string& code, mrbc_context* p_context)
		{
			return mrb_load_nstring_cxt(mp_mruby, code.c_str(), code.length(), p_context);
		}

		bool cMRuby_Interpreter::Run_Code(const std::string& code, const std::string& contextname)
		{
			// Create a new context. This is important so we
			// can properly retrieve exceptions, which mrb_load_string()
			// does not allow.
			mrbc_context* p_context = mrbc_context_new(mp_mruby);
			p_context->capture_errors = true;
			p_context->lineno = 1;
			mrbc_filename(mp_mruby, p_context, contextname.c_str()); // Set context filename (for exceptions)

			Run_Code_In_Context(code, p_context);

			bool result;
			if (mp_mruby->exc) {
				// Exception occured
				mrb_print_error(mp_mruby);
				result = false;
			}
			else
				result = true;

			mrbc_context_free(mp_mruby, p_context);
			return result;
		}

		bool cMRuby_Interpreter::Run_File(const boost::filesystem::path& filepath)
		{
			// Note we cannot use mrb_load_file(), because we use boost::filesystem’s
			// filereading capabilities which mruby doesn’t understand. Instead, we
			// simply pass the read file’s contents to mruby.

			// Open the file.
			boost::filesystem::ifstream file(filepath);
			if (!file.is_open()) {
				std::cerr << "Failed to open mruby script file '" << path_to_utf8(filepath) << "'" << std::endl;
				return false;
			}

			// Read it.
			std::string code = readfile(file);
			file.close();

			// Compile & execute it.
			return Run_Code(code, path_to_utf8(filepath.filename()).c_str());
		}

		void cMRuby_Interpreter::Load_Scripts()
		{
			// Create the SMC module
			Init_SMC(mp_mruby);

			// Load the main scripting file. This file is required to call
			// SMC::setup, which loads all of the wrapper classes into
			// the interpreter.
			boost::filesystem::path mainfile = pResource_Manager->Get_Game_Scripting("main.rb");

			/* If the main file errors, we cannot know which classes are loaded and
			 * which aren’t. We have no real information on the state of the scripting
			 * environment. This may obscurely break scripting-based levels or even
			 * segfault SMC. Hence we cannot ignore this case and must abort.
			 */
			// FIXME: Throw a proper exception, catch it on level loading and refuse to load the level.
			if (!Run_File(mainfile)) {
				std::string msg;
				msg.append("Error loading main mruby script '");
				msg.append(path_to_utf8(mainfile));
				msg.append("'. Cannot continue!");
				std::cerr << "FATAL: " << msg << std::endl;
				throw(msg);
			}
		}

		void cMRuby_Interpreter::Register_Callback(mrb_value callback)
		{
			// Note we need to lock the access to the list of callbacks
			// to prevent race conditions.
			boost::lock_guard<boost::mutex> _lock(m_callback_mutex);
			m_callbacks.push_back(callback);
		}

		void cMRuby_Interpreter::Evaluate_Timer_Callbacks()
		{
			// Note we need to lock the access to the list of callbacks
			// to prevent race conditions.
			boost::lock_guard<boost::mutex> _lock(m_callback_mutex);

			// Don’t put unnecessary strain in the mainloop (this method
			// is called once a frame!) if no timers are there.
			if (m_callbacks.empty())
				return;

			// Iterate through the list of registered callbacks
			// and evaluate each one
			std::vector<mrb_value>::iterator iter;
			for(iter = m_callbacks.begin(); iter != m_callbacks.end(); iter++) {
				mrb_funcall(mp_mruby, *iter, "call", 0);
				if (mp_mruby->exc) {
					std::cerr << "Warning: Error running timer callback: " << std::endl;
					mrb_print_error(mp_mruby);
				}
			}

			// Empty the list of registered callbacks. The timers
			// will add  to it again when necessary.
			m_callbacks.clear();
		}

		void cMRuby_Interpreter::Load_Wrappers(mrb_state* p_state)
		{
			using namespace SMC::Scripting;

			// When changing the order, ensure parent mruby classes get defined
			// prior to their mruby subclasses!
			Init_Eventable(p_state);
			Init_Sprite(p_state);
			Init_Moving_Sprite(p_state);
			Init_Animated_Sprite(p_state);
			Init_Level(p_state);
			Init_Level_Player(p_state);
			Init_Input(p_state);
			Init_Audio(p_state);
			Init_Timer(p_state);
			Init_Enemy(p_state);
			Init_Beetle(p_state);
			Init_BeetleBarrage(p_state);
			Init_Eato(p_state);
			Init_Flyon(p_state);
			Init_Furball(p_state);
			Init_Gee(p_state);
			Init_Krush(p_state);
			Init_Pip(p_state);
			Init_Rokko(p_state);
			Init_Spika(p_state);
			Init_Spikeball(p_state);
			Init_StaticEnemy(p_state);
			Init_Thromp(p_state);
			Init_Turtle(p_state);
			Init_TurtleBoss(p_state);
			Init_Larry(p_state);
			Init_Powerup(p_state);
			Init_Mushroom(p_state);
			Init_Fireplant(p_state);
			Init_Moon(p_state);
			Init_Star(p_state);
			Init_Box(p_state);
			Init_SpinBox(p_state);
			Init_TextBox(p_state);
			Init_BonusBox(p_state);
			Init_ParticleEmitter(p_state);
			Init_LevelExit(p_state);
			Init_LevelEntry(p_state);
			Init_Path(p_state);
			Init_Lava(p_state);
			Init_EnemyStopper(p_state);
			Init_Goldpiece(p_state);
			Init_JumpingGoldpiece(p_state);
			Init_Crate(p_state);
			Init_UIDS(p_state); // Call this last so it can rely on the other MRuby classes to be defined
		}
	}
}
