// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include <string>
#include "../script.h"
#include "downgrade_event.h"

namespace SMC{
	namespace Script{

		cDowngrade_Event::cDowngrade_Event(int downgrades, int max_downgrades)
		{
			m_downgrades		= downgrades;
			m_max_downgrades	= max_downgrades;
		}

		std::string cDowngrade_Event::Event_Name()
		{
			return "downgrade";
		}

		int cDowngrade_Event::Get_Downgrades()
		{
			return m_downgrades;
		}

		int cDowngrade_Event::Get_Max_Downgrades()
		{
			return m_max_downgrades;
		}

		int cDowngrade_Event::Run_Lua_Callback(cLua_Interpreter* p_lua)
		{
			lua_State* p_state = p_lua->Get_Lua_State();

			lua_pushnumber(p_state, m_downgrades);
			lua_pushnumber(p_state, m_max_downgrades);
			return lua_pcall(p_state, 2, 0, 0);
		}

	};
};
