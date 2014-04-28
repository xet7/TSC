#ifndef SMC_SCRIPTING_BEETLE_HPP
#define SMC_SCRIPTING_BEETLE_HPP
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcBeetle;
		void Init_Beetle(mrb_state* p_state);
	}
}

#endif
