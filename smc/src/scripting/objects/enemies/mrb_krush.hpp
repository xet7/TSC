#ifndef SMC_SCRIPTING_KRUSH_HPP
#define SMC_SCRIPTING_KRUSH_HPP
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcKrush;
		void Init_Krush(mrb_state* p_state);
	}
}
#endif
