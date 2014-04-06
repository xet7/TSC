#ifndef SMC_SCRIPTING_SPINBOX_HPP
#define SMC_SCRIPTING_SPINBOX_HPP
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcSpin_Box;
		void Init_SpinBox(mrb_state* p_state);
	}
}
#endif
