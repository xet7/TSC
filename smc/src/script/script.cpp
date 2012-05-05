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
#include "objects/l_level_player.h"
#include "objects/l_enemy.h"
#include "objects/l_furball.h"
#include "objects/l_eato.h"
#include "objects/l_flyon.h"
#include "objects/l_particle_emitter.h"
#include "objects/l_audio.h"
#include "objects/l_message.h"
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

		ClassMap type2class; // extern
		ClassMap class2type; // extern
		static void Open_Libs(lua_State* p_state);

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
			type2class[typeid(cLevel_Player).name()]		= "LevelPlayer";
			// Message has no C++ SMC counterpart
			type2class[typeid(cMovingSprite).name()]		= "MovingSprite";
			type2class[typeid(cParticle_Emitter).name()]	= "ParticleEmitter";
			type2class[typeid(cSprite).name()]				= "Sprite";

			// Invert the type2class table
			for (ClassMap::const_iterator iter = type2class.begin(); iter != type2class.end(); iter++)
				class2type[(*iter).second] = (*iter).first;
		}

		lua_State* New_Lua_State(const cLevel* p_level)
		{
			// Shorthand
			const cSprite_List& objs = p_level->m_sprite_manager->objects;

			// Create a new Lua interpreter
			lua_State* p_state = luaL_newstate();

			// Load the Lua main libraries and the SMC libraries
			luaL_openlibs(p_state);
			Open_Libs(p_state);

			// Build up the UID table
			cSprite_List::const_iterator iter;
			lua_newtable(p_state); // This is going to be the UID table
			for (iter = objs.begin(); iter != objs.end(); iter++){
				cSprite* p_sprite = *iter;

				/* Get the class table of the class this object shall be an
				 * instance of (needed for attaching the correct instance method
				 * table). If the object type hasn’t been bridged to Lua yet,
				 * fall back to `Sprite'. */
				std::string cpptypename = typeid(*p_sprite).name();
				if (type2class.count(cpptypename) > 0)
					lua_getglobal(p_state, type2class[cpptypename].c_str());
				else
					lua_getglobal(p_state, "Sprite");

				// Get the UID we want to register in the UID table.
				lua_pushnumber(p_state, p_sprite->m_uid);

				/* All pointers have the same size, therefore it’s unimportant
				 * what pointer’s size we pass to Lua. It is not important to
				 * know what it is when packaging the object, but when unwrapping
				 * it. */
				cSprite** pp_sprite	= (cSprite**) lua_newuserdata(p_state, sizeof(char*));
				*pp_sprite			= p_sprite;
				LuaWrap::InternalC::set_imethod_table(p_state, -3);

				// Add the UID table entry
				lua_settable(p_state, -4);

				// Remove the class table (we don’t need it anymore)
				lua_pop(p_state, 1);
			}
			lua_setglobal(p_state, "UIDS"); // Make the table globally available

			// Return the Lua instance
			return p_state;
		}

		void Cleanup_Lua_State(lua_State* p_state)
		{
			lua_close(p_state);
		}

		/** Opens all SMC-specific Lua libraries. */
		static void Open_Libs(lua_State* p_state)
		{
			// In-game object bindings
			Open_Sprite(p_state);
			Open_Moving_Sprite(p_state);
			Open_Animated_Sprite(p_state);
			Open_Level_Player(p_state);
			Open_Enemy(p_state);
			Open_Furball(p_state);
			Open_Eato(p_state);
			Open_Flyon(p_state);

			// Events
			Open_Key_Down_Event(p_state);

			// Other
			Open_Particle_Emitter(p_state);
			Open_Audio(p_state);
			Open_Message(p_state);
		}

	};
};
