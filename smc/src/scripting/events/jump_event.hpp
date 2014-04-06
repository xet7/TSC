#ifndef SMC_SCRIPTING_JUMP_EVENT_H
#define SMC_SCRIPTING_JUMP_EVENT_H
#include "../scripting.hpp"
#include "event.hpp"

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
