#include "luawrap.hpp"
#include "lua_level_player.h"
#include "events.h"
#include "lualibs.h"

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
 */

namespace SMC
{
	namespace Lua
	{
		void openlibs(lua_State* p_state)
		{
			open_level_player(p_state);
			open_events(p_state);
		}

	};
};
