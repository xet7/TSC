// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../script.h"
#include "../../objects/sprite.h"
#include "sprite_event.h"
#include <iostream>

namespace SMC{
	namespace Script{

		std::string cSprite_Event::Event_Name()
		{
			return "generic_sprite";
		}

		/**
		 * Cycles through all event handler functions registered for
		 * this event (see Event_Name()) in the given sprite and
		 * for each of them, calls Run_Lua_Callback() (see further
		 * down) after the function has been pushed onto the Lua
		 * stack.
		 */
		void cSprite_Event::Fire(cLua_Interpreter* p_lua, cSprite* p_sprite)
		{
			using namespace std;

			// Menu level has no Lua interpreter
			if (!p_lua)
				return;

			lua_State* p_state = p_lua->Get_Lua_State();
			vector<int> handlers = p_sprite->m_event_table[Event_Name()];

			for(vector<int>::const_iterator iter = handlers.begin(); iter < handlers.end(); iter++){
				lua_rawgeti(p_state, LUA_REGISTRYINDEX, *iter);
				if (Run_Lua_Callback(p_lua) != LUA_OK){
					cerr << "Error running Lua handler: " << lua_tostring(p_state, -1) << std::endl;
					lua_pop(p_state, 1); // Remove the error message from the stack
				}
			}
		}

		/** Deligates to the superclass method. */
		int cSprite_Event::Run_Lua_Callback(cLua_Interpreter* p_lua)
		{
			return cEvent::Run_Lua_Callback(p_lua);
		}

	};
};
