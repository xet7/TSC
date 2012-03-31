// -*- c++ -*-
#ifndef SMC_SCRIPT_TOUCH_EVENT_H
#define SMC_SCRIPT_TOUCH_EVENT_H
#include <string>
#include "../luawrap.hpp"
#include "../../objects/sprite.h"
#include "sprite_event.h"

namespace SMC{
	namespace Script{

		class cTouch_Event: public cSprite_Event
		{
		public:
			cTouch_Event(cSprite* p_collided);
			virtual std::string Event_Name();
			cSprite* Get_Collided();
		protected:
			virtual int Run_Lua_Callback(lua_State* p_state);
		private:
			cSprite* mp_collided;
		};

	};
};
#endif
