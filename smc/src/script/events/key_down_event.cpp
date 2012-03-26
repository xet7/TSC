#include "event.h"
#include "key_down_event.h"

namespace SMC{
	namespace Script{

		/* *** *** *** *** *** cKeyDownEvent *** *** *** *** *** *** *** *** *** *** *** *** */

		/***************************************
		 * C++ implementation
		 ***************************************/

		cKeyDownEvent::cKeyDownEvent(std::string keyname)
		{
			m_keyname = keyname;
		}

		std::string cKeyDownEvent::Get_Keyname()
		{
			return m_keyname;
		}

		/***************************************
		 * Lua binding
		 ***************************************/

		static int lua_key_down_event_alloc(lua_State* p_state)
		{
			if (!lua_istable(p_state, 1))
				return luaL_error(p_state, "No class table given.");

			std::string keyname = luaL_checkstring(p_state, 2);
			cKeyDownEvent* p_event = new (lua_newuserdata(p_state, sizeof(cKeyDownEvent))) cKeyDownEvent(keyname);

			if (!p_event)
				return luaL_error(p_state, "Failed to allocate memory.");

			return 1;
		}

		static int lua_key_down_get_keyname(lua_State* p_state)
		{
			cKeyDownEvent* p_event = LuaWrap::to<cKeyDownEvent>(p_state);
			lua_pushstring(p_state, p_event->Get_Keyname().c_str());
			return 1;
		}

		static luaL_Reg key_down_event_cmethods[] = {
			{"register", lua_event_register<cKeyDownEvent>},
			{NULL, NULL}
		};

		static luaL_Reg key_down_event_imethods[] = {
			{"keyname", lua_key_down_get_keyname},
			{NULL, NULL}
		};

		/* *** *** *** *** *** namespace *** *** *** *** *** *** *** *** *** *** *** *** */
		void Open_Key_Down_Event(lua_State* p_state)
		{
			LuaWrap::register_class<cKeyDownEvent>(p_state, "KeyDownEvent", key_down_event_imethods, key_down_event_cmethods, NULL, NULL);
		}
	};
};
