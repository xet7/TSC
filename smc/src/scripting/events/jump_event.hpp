#ifndef SMC_SCRIPTING_JUMP_EVENT_HPP
#define SMC_SCRIPTING_JUMP_EVENT_HPP
#include "../scripting.hpp"
#include "event.hpp"

namespace SMC {
	namespace Scripting {

		class cJump_Event: public cEvent
		{
		public:
			virtual string Event_Name(){return "jump";}
		};
	}
}

#endif
