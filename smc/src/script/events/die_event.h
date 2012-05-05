// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPT_DIE_H
#define SMC_SCRIPT_DIE_H
#include <string>
#include "../script.h"
#include "sprite_event.h"
namespace SMC{
	namespace Script{
		class cDie_Event: public cSprite_Event
		{
		public:
			virtual std::string Event_Name(){return "die";}
		};
	};
};
#endif
