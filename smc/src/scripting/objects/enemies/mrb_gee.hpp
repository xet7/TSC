#ifndef SMC_SCRIPTING_GEE_HPP
#define SMC_SCRIPTING_GEE_HPP
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcGee;
		void Init_Gee(mrb_state* p_state);
	}
}
#endif
