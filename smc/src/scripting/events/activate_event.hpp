#ifndef SMC_SCRIPTING_ACTIVATE_EVENT_HPP
#define SMC_SCRIPTING_ACTIVATE_EVENT_HPP
#include "../scripting.hpp"
#include "event.hpp"

namespace SMC {
	namespace Scripting {
		class cActivate_Event: public cEvent
		{
		public:
			virtual string Event_Name(){return "activate";}
		};
	}
}
#endif
