#include <string>
#include "../luawrap.hpp"
#include "downgrade_event.h"

namespace SMC{
	namespace Script{

		cDowngrade_Event::cDowngrade_Event(int downgrades, int max_downgrades)
		{
			m_downgrades		 = downgrades;
			m_max_downgrades = max_downgrades;
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

		int cDowngrade_Event::Run_Lua_Callback(lua_State* p_state)
		{
			lua_pushnumber(p_state, m_downgrades);
			lua_pushnumber(p_state, m_max_downgrades);
			return lua_pcall(p_state, 2, 0, 0);
		}

	};
};
