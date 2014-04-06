// -*- c++ -*-
#ifndef SMC_SCRIPTING_DIE_EVENT_H
#define SMC_SCRIPTING_DIE_EVENT_H
#include "../scripting.h"
#include "event.h"

namespace SMC {
	namespace Scripting {

		class cDie_Event: public cEvent
		{
		public:
			virtual std::string Event_Name(){return "die";}
		};
	}
}

#endif
