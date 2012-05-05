// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPT_SHOOT_EVENT_H
#define SMC_SCRIPT_SHOOT_EVENT_H
#include <string>
#include "../script.h"
#include "sprite_event.h"

namespace SMC{
	namespace Script{
		class cShoot_Event: public cSprite_Event
		{
		public:
			cShoot_Event();
			cShoot_Event(std::string ball_type);
			virtual std::string Event_Name();
			std::string Get_Ball_Type();
			void Set_Ball_Type(std::string ball_type);
		protected:
			virtual int Run_Lua_Callback(cLua_Interpreter* p_lua);
		private:
			std::string m_ball_type;
		};
	};
};
#endif
