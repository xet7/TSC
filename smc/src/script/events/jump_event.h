// -*- c++ -*-
#ifndef SMC_SCRIPT_JUMP_EVENT_H
#define SMC_SCRIPT_JUMP_EVENT_H
#include <string>
#include "../luawrap.hpp"
#include "sprite_event.h"

namespace SMC{
	namespace Script{
		class cJump_Event: public cSprite_Event
		{
		public:
			virtual std::string Event_Name(){return "jump";}
		};
	};
};
#endif
