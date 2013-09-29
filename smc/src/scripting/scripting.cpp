#include "scripting.h"
#include "../level/level.h"
#include "../level/level_player.h"
#include "../core/sprite_manager.h"
#include "../core/property_helper.h"
#include "../core/filesystem/resource_manager.h"

#include "objects/mrb_smc.h"
#include "objects/mrb_eventable.h"
#include "objects/mrb_uids.h"
#include "objects/sprites/mrb_sprite.h"
#include "objects/sprites/mrb_moving_sprite.h"
#include "objects/sprites/mrb_animated_sprite.h"
#include "objects/sprites/mrb_particle_emitter.h"
#include "objects/misc/mrb_level.h"
#include "objects/misc/mrb_level_player.h"
#include "objects/misc/mrb_audio.h"
#include "objects/misc/mrb_input.h"
#include "objects/misc/mrb_timer.h"
#include "objects/enemies/mrb_enemy.h"
#include "objects/enemies/mrb_eato.h"
#include "objects/enemies/mrb_flyon.h"
#include "objects/enemies/mrb_furball.h"
#include "objects/enemies/mrb_gee.h"
#include "objects/enemies/mrb_krush.h"
#include "objects/enemies/mrb_rokko.h"
#include "objects/enemies/mrb_spika.h"
#include "objects/enemies/mrb_spikeball.h"
#include "objects/enemies/mrb_static.h"
#include "objects/enemies/mrb_thromp.h"
#include "objects/enemies/mrb_turtle.h"
#include "objects/enemies/mrb_turtle_boss.h"
#include "objects/powerups/mrb_powerup.h"
#include "objects/powerups/mrb_mushroom.h"
#include "objects/powerups/mrb_fireplant.h"
#include "objects/powerups/mrb_moon.h"
#include "objects/powerups/mrb_star.h"
#include "objects/boxes/mrb_box.h"
#include "objects/boxes/mrb_spinbox.h"
#include "objects/boxes/mrb_textbox.h"
#include "objects/boxes/mrb_bonusbox.h"
#include "objects/specials/mrb_level_exit.h"
#include "objects/specials/mrb_path.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

/*****************************************************************************
 Scripting organisation

Yeah, scripting! SMC embeds mruby (https://github.com/mruby/mruby), a minimal
Ruby implementation for boosting your level design. The cMRubyInterpreter class
wraps the mruby_state struct in a more convenient way and also initialises it for
use with SMC. Each time a level is loaded, a new clean instance of this class
is created (and of course destroyed on level ending).

After the setup is done, the constructor calls the Load_Scripts()
private member which in turn feeds the "main.rb" file in the scripting/ directory
of your SMC installation into the mruby interpreter. This script, which may
be added by users if they want to provide additional functionality globally,
has access to a very minimalistic version of #require, which just allows for
loading scripts relative to the scripting/ directory and also has some quirks,
such as the inability to define toplevel constants without an explicit :: at
the beginning, e.g.

  module Foo
  end

will not work, but

  module ::Foo
  end

will. The default main.rb file only contains the bare minimum needed to
correctly initialise the scripting functionality and users are advised to
not remove this code. Removing e.g. the Eventable module will have very bad
effects, but on the other hand this gives you the full power of Ruby to
hook in everything you want to. Finally, the main.rb script is required to
call the SMC::setup method, which loads all the C++ wrapper classes (i.e.
Sprite, LevelPlayer, etc.) into the interpreter.

*****************************************************************************/



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

		bool cMRuby_Interpreter::Run_Code(const std::string& code, std::string& errormsg)
		{
			mrb_load_string(mp_mruby, code.c_str());
			if (mp_mruby->exc){
				errormsg = format_mruby_error(mp_mruby, mp_mruby->exc);
				return false;
			}
			else
				return true;
		}

		void cMRuby_Interpreter::Load_Scripts()
		{
			// Create the SMC module
			Init_SMC(mp_mruby);

			// Load the main scripting file. This file is required to call
			// SMC::setup, which loads all of the wrapper classes into
			// the interpreter.
			boost::filesystem::path mainfile = pResource_Manager->Get_Game_Scripting("main.rb");
			boost::filesystem::ifstream file(mainfile);
			if (!file.is_open()) {
				std::cerr << "Failed to open main scripting file '" << path_to_utf8(mainfile) << "' for reading." << std::endl;
				std::cerr << "Scripting will not work correctly." << std::endl;
				return;
			}
			else {
				mrb_load_string(mp_mruby, readfile(file).c_str());
				if (mp_mruby->exc){
					std::cerr << "Warning: Error loading main script: ";
					std::cerr << format_mruby_error(mp_mruby, mp_mruby->exc);
					std::cerr << std::endl;
				}
				file.close();
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
					std::cerr << "Warning: Error running timer callback: ";
					std::cerr << format_mruby_error(mp_mruby, mp_mruby->exc);
					std::cerr << std::endl;
				}
			}

			// Empty the list of registered callbacks. The timers
			// will add  to it again when necessary.
			m_callbacks.clear();
		}

	}

}

void SMC::Scripting::Load_Wrappers(mrb_state* p_state)
{
	using namespace SMC::Scripting;

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
	Init_Eato(p_state);
	Init_Flyon(p_state);
	Init_Furball(p_state);
	Init_Gee(p_state);
	Init_Krush(p_state);
	Init_Rokko(p_state);
	Init_Spika(p_state);
	Init_Spikeball(p_state);
	Init_StaticEnemy(p_state);
	Init_Thromp(p_state);
	Init_Turtle(p_state);
	Init_TurtleBoss(p_state);
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
	Init_Path(p_state);
	Init_UIDS(p_state); // Call this last so it can rely on the other MRuby classes to be defined
}
