#ifndef SMC_SCRIPTING_ENTER_EVENT_HPP
#define SMC_SCRIPTING_ENTER_EVENT_HPP
#include "../scripting.hpp"
#include "event.hpp"

namespace SMC {

	namespace Scripting {

		class cEnter_Event: public cEvent
		{
		public:
			virtual string Event_Name(){return "enter";}
		};

	}

}

#endif
