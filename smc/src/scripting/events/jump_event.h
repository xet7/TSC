// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPTING_JUMP_EVENT_H
#define SMC_SCRIPTING_JUMP_EVENT_H
#include "../scripting.h"
#include "event.h"

namespace SMC {
	namespace Scripting {

		class cJump_Event: public cEvent
		{
		public:
			virtual std::string Event_Name(){return "jump";}
		};
	}
}

#endif
