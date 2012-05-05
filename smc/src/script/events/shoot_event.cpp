// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../script.h"
#include "shoot_event.h"

namespace SMC{
	namespace Script{

		cShoot_Event::cShoot_Event()
		{
			m_ball_type = "fire";
		}

		cShoot_Event::cShoot_Event(std::string ball_type)
		{
			m_ball_type = ball_type;
		}

		std::string cShoot_Event::Event_Name()
		{
			return "shoot";
		}

		int cShoot_Event::Run_Lua_Callback(cLua_Interpreter* p_lua)
		{
      lua_State* p_state = p_lua->Get_Lua_State();

			lua_pushstring(p_state, m_ball_type.c_str());
			return lua_pcall(p_state, 1, 0, 0);
		}

		std::string cShoot_Event::Get_Ball_Type()
		{
			return m_ball_type;
		}

		void cShoot_Event::Set_Ball_Type(std::string ball_type)
		{
			m_ball_type = ball_type;
		}

	};
};
