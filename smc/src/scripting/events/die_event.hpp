// -*- c++ -*-
#ifndef SMC_SCRIPTING_DIE_EVENT_H
#define SMC_SCRIPTING_DIE_EVENT_H
#include "../scripting.hpp"
#include "event.hpp"

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
