// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include <string>
#include "../luawrap.hpp"
#include "../../objects/sprite.h"
#include "touch_event.h"

namespace SMC{
	namespace Script{

		cTouch_Event::cTouch_Event(cSprite* p_collided)
		{
			mp_collided = p_collided;
		}

		std::string cTouch_Event::Event_Name()
		{
			return "touch";
		}

		cSprite* cTouch_Event::Get_Collided()
		{
			return mp_collided;
		}

		int cTouch_Event::Run_Lua_Callback(lua_State* p_state)
		{
			// Look the UID of the colliding sprite up in the global UIDS table
			// and push the corresponding Lua object onto the stack
			lua_getglobal(p_state, "UIDS");
			lua_pushnumber(p_state, mp_collided->m_uid);
			lua_gettable(p_state, -2);

			// Remove the UIDS table from the stack
			lua_insert(p_state, -2);
			lua_pop(p_state, 1);

			// Call the handler with the Lua object as the sole parameter
			return lua_pcall(p_state, 1, 0, 0);
		}

	};
};
