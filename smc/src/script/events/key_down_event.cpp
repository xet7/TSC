#include "event.h"
#include "key_down_event.h"

namespace SMC{
	namespace Script{

		/* *** *** *** *** *** cKeyDownEvent *** *** *** *** *** *** *** *** *** *** *** *** */

		cKeyDownEvent::cKeyDownEvent(std::string keyname)
		{
			m_keyname = keyname;
		}

		std::string cKeyDownEvent::Get_Keyname()
		{
			return m_keyname;
		}

		// Overwrite
		int cKeyDownEvent::Run_Lua_Callback(lua_State* p_state)
		{
			lua_pushstring(p_state, m_keyname.c_str());
			return lua_pcall(p_state, 1, 0, 0);
		}

		static luaL_Reg key_down_event_methods[] = {
			{"register", lua_event_register<cKeyDownEvent>},
			{NULL, NULL}
		};

		/* *** *** *** *** *** namespace *** *** *** *** *** *** *** *** *** *** *** *** */
		void Open_Key_Down_Event(lua_State* p_state)
		{
			LuaWrap::register_singleton(p_state, "KeyDownEvent", key_down_event_methods);
		}
	};
};
