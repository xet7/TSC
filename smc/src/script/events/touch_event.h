// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPT_TOUCH_EVENT_H
#define SMC_SCRIPT_TOUCH_EVENT_H
#include <string>
#include "../script.h"
#include "../../objects/sprite.h"
#include "event.h"

namespace SMC{
	namespace Script{

		class cTouch_Event: public cEvent
		{
		public:
			cTouch_Event(cSprite* p_collided);
			virtual std::string Event_Name();
			cSprite* Get_Collided();
		protected:
			virtual int Run_Lua_Callback(cLua_Interpreter* p_lua);
		private:
			cSprite* mp_collided;
		};

	};
};
#endif
