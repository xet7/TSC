// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include <iostream>
#include <vector>
#include "../script.h"
#include "event.h"

namespace SMC{
	namespace Script{

		/* *** *** *** *** *** cEvent *** *** *** *** *** *** *** *** *** *** *** *** */

		// Static member definition
		std::vector<int>cEvent::M_handlers;

		/**
		 * Registers a Lua callback function for this event.
		 * registryindex is the index of the function in the
		 * Lua registry which is created in the function
		 * binding this function to Lua.
		 */
		void cEvent::Register_Handler(int registryindex)
		{
			M_handlers.push_back(registryindex);
		}

		/**
		 * Cycles through all registered event handlers and
		 * calls the instance method cEvent::Run_Lua_Callback()
		 * for them. See Run_Lua_Callback()’s documentation for
		 * more information on this.
		 *
		 * For subclasses, you usually don’t want to overwrite Fire(),
		 * but rather Run_Lua_Callback().
		 */
		void cEvent::Fire(cLua_Interpreter* p_lua)
		{
			// Menu level has no Lua interpreter
			if (!p_lua)
				return;

			lua_State* p_state = p_lua->Get_Lua_State();

			for(std::vector<int>::iterator iter = M_handlers.begin(); iter < M_handlers.end(); iter++){
				lua_rawgeti(p_state, LUA_REGISTRYINDEX, *iter);
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
};
