#ifndef SMC_SCRIPTING_SPIKEBALL_H
#define SMC_SCRIPTING_SPIKEBALL_H
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcSpikeball;
		void Init_Spikeball(mrb_state* p_state);
	}
}
#endif
