// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include <iostream>
#include <vector>
#include "../script.h"
#include "../scriptable_object.h"
#include "event.h"

namespace SMC{
	namespace Script{

		/* *** *** *** *** *** cEvent *** *** *** *** *** *** *** *** *** *** *** *** */

		/**
		 * Cycles through all registered event handlers and
		 * calls the instance method cEvent::Run_Lua_Callback()
		 * for them. See Run_Lua_Callback()’s documentation for
		 * more information on this.
		 *
		 * For subclasses, you usually don’t want to overwrite Fire(),
		 * but rather Run_Lua_Callback().
		 */
		void cEvent::Fire(cLua_Interpreter* p_lua, cScriptable_Object* p_obj)
		{
			// Menu level has no Lua interpreter
			if (!p_lua)
				return;

			// Get the Lua state and the list of callback function
			// indices
			lua_State* p_state = p_lua->Get_Lua_State();
			std::vector<int> handlers = p_obj->m_event_table[Event_Name()];
			std::vector<int>::iterator iter;

			// Iterate through the list of registered callback indices
			// and call their corresponding functions.
			for(iter = handlers.begin(); iter != handlers.end(); iter++){
				lua_rawgeti(p_state, LUA_REGISTRYINDEX, *iter); // Push the function onto the stack
				if (Run_Lua_Callback(p_lua) != LUA_OK){
					std::cerr << "Error running Lua handler: " << lua_tostring(p_state, -1) << std::endl;
					lua_pop(p_state, 1); // Remove the error message from the stack
				}
			}
		}

		/**
		 * Called whenever a Lua callback function shall be run.
		 * When this is called, the callback Lua function already
		 * is on the stack at index -1, i.e. at the top. It should
		 * forward the result of the call to lua_pcall() as the return
		 * value.
		 *
		 * The default implementation in the cEvent class just calls
		 * the function without any arguments.
		 */
		int cEvent::Run_Lua_Callback(cLua_Interpreter* p_lua)
		{
			return lua_pcall(p_lua->Get_Lua_State(), 0, 0, 0);
		}

		/**
		 * Returns the name of the event, used for firing the event
		 * to the correct event handler functions. Subclasses must
		 * override this with a proper event name. The names returned
		 * by these functions define the names of the event handlers
		 * on the Lua side, i.e. the "touch" event corresponds to an
		 * on_touch() register method.
		 */
		std::string cEvent::Event_Name()
		{
			return "generic";
		}

	};

	/**
	 * Helper method that just forwards a Lua call to register() with
	 * the given name. Expects the given stack to look like this:
	 *	 [1] Receiver (self, a userdata)
	 *	 [2] The Lua function handler
	 *
	 * The IMPLEMENT_LUA_EVENT macro just defines a function that
	 * calls this function with the argument given to it converted
	 * to a string.
	 */
	int SMC::Script::Forward_To_Register(lua_State* p_state, std::string event_name)
	{
		if (!lua_isuserdata(p_state, 1)) // self
			return luaL_error(p_state, "No receiver (userdata) given.");
		if (!lua_isfunction(p_state, 2)) // handler function
			return luaL_error(p_state, "No function given.");

		// Get the register() function
		lua_pushstring(p_state, "register");
		lua_gettable(p_state, 1);
		// Forward to register()
		lua_pushvalue(p_state, 1); // self
		lua_pushstring(p_state, event_name.c_str());
		lua_pushvalue(p_state, 2); // function
		lua_call(p_state, 3, 0);

		return 0;
	}
};
