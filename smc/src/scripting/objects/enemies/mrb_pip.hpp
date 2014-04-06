#ifndef SMC_SCRIPTING_PIP_HPP
#define SMC_SCRIPTING_PIP_HPP
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcPip;
		void Init_Pip(mrb_state* p_state);
	}
}
#endif
