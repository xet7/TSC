#ifndef SMC_SCRIPTING_SPIT_EVENT_HPP
#define SMC_SCRIPTING_SPIT_EVENT_HPP
#include "../scripting.hpp"
#include "event.hpp"

namespace SMC {
	namespace Scripting {
		class cSpit_Event: public cEvent
		{
		public:
			virtual std::string Event_Name(){return "spit";}
		};
	}
}
#endif
