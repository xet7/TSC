// -*- c++ -*-
#ifndef SMC_SCRIPT_SPRITE_EVENT_H
#define SMC_SCRIPT_SPRITE_EVENT_H
#include <string>
#include "../luawrap.hpp"
#include "event.h"

namespace SMC{
	namespace Script{
		class cSprite_Event: public cEvent
		{
		public:
			virtual std::string Event_Name();
			void Fire(lua_State* p_state, cSprite* p_sprite);
		protected:
			virtual int Run_Lua_Callback(lua_State* p_state);
		};
	};
};

#endif
