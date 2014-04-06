#ifndef SMC_SCRIPTING_BOX_HPP
#define SMC_SCRIPTING_BOX_HPP
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcBox;
		void Init_Box(mrb_state* p_state);
	}
}
#endif
