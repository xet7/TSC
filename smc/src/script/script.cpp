// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../level/level.h"
#include "../core/sprite_manager.h"
// Own header
#include "script.h"
// Used classes
#include "../audio/audio.h"
#include "../enemies/eato.h"
#include "../enemies/enemy.h"
#include "../enemies/flyon.h"
#include "../enemies/furball.h"
#include "../enemies/gee.h"
#include "../level/level_player.h"
#include "../objects/sprite.h"
// In-game object bindings
#include "objects/l_sprite.h"
#include "objects/l_moving_sprite.h"
#include "objects/l_animated_sprite.h"
#include "objects/l_level.h"
#include "objects/l_level_player.h"
#include "objects/l_enemy.h"
#include "objects/l_furball.h"
#include "objects/l_eato.h"
#include "objects/l_flyon.h"
#include "objects/l_gee.h"
#include "objects/l_particle_emitter.h"
#include "objects/l_audio.h"
#include "objects/l_input.h"
#include "objects/l_message.h"
#include "objects/l_timers.h"
// Events
#include "events/event.h"
#include "events/key_down_event.h"

/* SMC embeds Lua as a scripting language the user might
 * find useful to add uncommon features to his levels.
 * The scripting mechanism is centralized and event-based,
 * i.e. inside each level there is exactly *one* script to
 * be found that registers callback functions (so-called
 * "handler functions" or short "handlers") for a set of
 * predefined events SMC issues from time to time. 
 *
 * Events are organized in a hierarchical set of event
 * classes that exists on both the C++ and the Lua side.
 * The C++ classes, declared in events.h, each hold a
 * list of Lua callback functions registered for the
 * given event and that will be called whenever SMC
 * creates an event and calls its fire() method.
 * Callback functions are regisred from Lua by calling
 * the respective event class’ register() class-method
 * that stores the given function in Lua’s registry and
 * adds the returned reference to the class’ list of
 * registered event handlers (by calling the C++
 * register_handler() method of the requested event class).
 *
 * Each event class may have its own needs regarding the
 * invocation of the Lua callback, especially regarding
 * arguments passed to it. To achieve this, the generic
 * Event class’ fire() method just cycles through all
 * registered event handlers and for each of them calls
 * out for the run_lua_callback() method you can overwrite
 * in your Event subclasses. See the already defined event
 * classes for examples.
 *
 * For an easy dynamic mapping of Lua to C++ classnames and
 * vice-versa, the class2type and type2class hash tables
 * (std::maps) are used. They map the internal C++ class names
 * (obtained via `typeid'; as the names are only used internally
 * within one compilation, the different names across different
 * compilers and compiler versions don’t matter) to the identifiers
 * given to the Lua classes whose instances wrap instances of these
 * C++ classes. Or simply think of a 1:1 mapping, although this
 * is not 100% accurate.
 */

namespace SMC
{
	namespace Script
	{
		/* *** *** *** *** *** cLua_Interpreter *** *** *** *** *** *** *** *** *** *** *** *** */

		cLua_Interpreter::cLua_Interpreter(cLevel* p_level)
		{
			// Shorthand
			cSprite_List& objs = p_level->m_sprite_manager->objects;

			// Set member variables
			mp_level					= p_level;
			mp_lua						= luaL_newstate(); // Create a new Lua instance
			mp_callback_indices_mutex	= new boost::mutex;
			mp_callback_indices			= new std::vector<int>;
			mp_timers					= new TimerList;

			// Load the Lua main libraries and the SMC libraries
			luaL_openlibs(mp_lua);
			Open_SMC_Libs();

			// Build up the UID table
			cSprite_List::iterator iter;
			lua_newtable(mp_lua); // This is going to be the UID table
			for (iter = objs.begin(); iter != objs.end(); iter++){
				cSprite* p_sprite = *iter;

				// Get the UID we want to register in the UID table.
				lua_pushnumber(mp_lua, p_sprite->m_uid);

				// Create the Lua full userdata object for the sprite
				Wrap_Lua_Object_Around_Sprite(*p_sprite);

				// Add the UID table entry
				lua_settable(mp_lua, -3);
			}
			// UID 0 is the player
			lua_pushnumber(mp_lua, 0);
			Wrap_Lua_Object_Around_Sprite(*pLevel_Player);
			lua_settable(mp_lua, -3);
			// Make the table globally available
			lua_setglobal(mp_lua, UID_TABLE_NAME.c_str());
		}

		cLua_Interpreter::~cLua_Interpreter()
		{
			// Stop all ticking timers and wait for them
			// to finish.
			TimerList::iterator iter;
			for(iter = mp_timers->begin(); iter != mp_timers->end(); iter++)
				(*iter)->Stop();

			delete mp_timers;
			delete mp_callback_indices_mutex;
			delete mp_callback_indices;
			lua_close(mp_lua);
		}

		lua_State* cLua_Interpreter::Get_Lua_State()
		{
			return mp_lua;
		}

		cLevel* cLua_Interpreter::Get_Level()
		{
			return mp_level;
		}

		bool cLua_Interpreter::Run_Code(const std::string& code, std::string& errormsg)
		{
			// luaL_dostring returns false in case of success, quite confusing)
			if (luaL_dostring(mp_lua, code.c_str())){
				errormsg = std::string(lua_tostring(mp_lua, -1));
				lua_pop(mp_lua, -1); // Remove the error message
				return false;
			}

			return true;
		}

		void cLua_Interpreter::Add_To_UID_Table(cSprite& sprite)
		{
			// TODO: Throw error when UID already in use?

			lua_getglobal(mp_lua, UID_TABLE_NAME.c_str());
			lua_pushnumber(mp_lua, sprite.m_uid);
			Wrap_Lua_Object_Around_Sprite(sprite);
			lua_settable(mp_lua, -3);
			lua_pop(mp_lua, 1); // Remove the UID table from the stack

		}

		void cLua_Interpreter::Register_Callback_Index(int registryindex)
		{
			// Note we need to lock the access to the list of
			// callbacks to prevent race conditions.
			boost::lock_guard<boost::mutex> _lock(*mp_callback_indices_mutex);
			mp_callback_indices->push_back(registryindex);
		}

		void cLua_Interpreter::Evaluate_Timer_Callbacks()
		{
			// Note we need to lock the access to the list of
			// callbacks to prevent race conditions.
			boost::lock_guard<boost::mutex> _lock(*mp_callback_indices_mutex);
			std::vector<int>::iterator iter;

			// Iterate through the list of registered callbacks
			// and evaluate each one
			for(iter = mp_callback_indices->begin(); iter != mp_callback_indices->end(); iter++){
				// Push a copy of the callback onto the stack
				lua_rawgeti(mp_lua, LUA_REGISTRYINDEX, *iter);

				// Execute it!
				switch(lua_pcall(mp_lua, 0, 0, 0)){
				case LUA_OK:
					break;
				case LUA_ERRMEM:
					// Memory errors are severe and cannot be ignored.
					// Immediately end SMC.
					std::cerr << "FATAL: Memory error when evaluating Lua callback!" << std::endl;
					exit(-1);
				case LUA_ERRRUN:	// Fallthrough
				case LUA_ERRGCMM:	// Fallthrough
				default:
					std::string errmsg = lua_tostring(mp_lua, -1);
					lua_pop(mp_lua, -1); // Remove the error message
					std::cerr << "Warning: Lua Error running callback: " << errmsg << std::endl;
				}
			}

			// Empty the list of registered callbacks. The timers
			// will add to it again when necessary.
			mp_callback_indices->clear();
		}

		void cLua_Interpreter::Register_Timer(cTimer* p_timer)
		{
			mp_timers->push_back(p_timer);
		}

		void cLua_Interpreter::Open_SMC_Libs()
		{
			// Sprites
			Open_Sprite(mp_lua);
			Open_Moving_Sprite(mp_lua);
			Open_Animated_Sprite(mp_lua);
			Open_Level(mp_lua);
			Open_Level_Player(mp_lua);
			Open_Enemy(mp_lua);
			Open_Furball(mp_lua);
			Open_Eato(mp_lua);
			Open_Flyon(mp_lua);
			Open_Gee(mp_lua);

			// Other
			Open_Particle_Emitter(mp_lua);
			Open_Audio(mp_lua);
			Open_Input(mp_lua);
			Open_Message(mp_lua);
			Open_Timers(mp_lua);
		}

		void cLua_Interpreter::Wrap_Lua_Object_Around_Sprite(cSprite& sprite)
		{
			/* Get the class table of the class this object shall be an
			 * instance of (needed for attaching the correct instance method
			 * table). If the object type hasn’t been bridged to Lua yet,
			 * fall back to `Sprite'. */
			std::string cpptypename = typeid(sprite).name();
			if (type2class.count(cpptypename) > 0)
				lua_getglobal(mp_lua, type2class[cpptypename].c_str());
			else
				lua_getglobal(mp_lua, "Sprite");

			/* All pointers have the same size, therefore it’s unimportant
			 * what pointer’s size we pass to Lua. It is not important to
			 * know what it is when packaging the object, but when unwrapping
			 * it. */
			cSprite** pp_sprite	= (cSprite**) lua_newuserdata(mp_lua, sizeof(char*));
			*pp_sprite			= &sprite;
			LuaWrap::InternalC::set_imethod_table(mp_lua, -2); // Attach instance methods

			// Remove the class table
			lua_insert(mp_lua, -2);
			lua_pop(mp_lua, 1);
		}

		/* *** *** *** *** *** Namespace members  *** *** *** *** *** *** *** *** *** *** *** *** */

		ClassMap type2class; // extern
		ClassMap class2type; // extern

		void Initialize_Scripting()
		{
			/* List of classes available in the SMC scripting interface.
			 * TODO: Any easier way to create this list beside manually
			 * modifying it? */
			type2class[typeid(cAnimated_Sprite).name()]		= "AnimatedSprite";
			type2class[typeid(cAudio).name()]				= "AudioClass";
			type2class[typeid(cEato).name()]				= "Eato";
			type2class[typeid(cEnemy).name()]				= "Enemy";
			type2class[typeid(cFlyon).name()]				= "Flyon";
			type2class[typeid(cFurball).name()]				= "Furball";
			type2class[typeid(cGee).name()]					= "Gee";
			// Input has multiple C++ counterparts
			type2class[typeid(cLevel).name()]				= "LevelClass";
			type2class[typeid(cLevel_Player).name()]		= "LevelPlayer";
			// Message has no C++ SMC counterpart
			type2class[typeid(cMovingSprite).name()]		= "MovingSprite";
			type2class[typeid(cParticle_Emitter).name()]	= "ParticleEmitter";
			type2class[typeid(cSprite).name()]				= "Sprite";
			type2class[typeid(cTimer).name()]				= "Timer";

			// Invert the type2class table
			for (ClassMap::const_iterator iter = type2class.begin(); iter != type2class.end(); iter++)
				class2type[(*iter).second] = (*iter).first;
		}

	};
};
