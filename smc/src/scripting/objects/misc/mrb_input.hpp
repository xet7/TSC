#ifndef SMC_SCRIPTING_INPUT_HPP
#define SMC_SCRIPTING_INPUT_HPP
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcInput;
		void Init_Input(mrb_state* p_state);
	}
}

#endif
