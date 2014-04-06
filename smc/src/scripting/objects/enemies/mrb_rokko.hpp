#ifndef SMC_SCRIPTING_ROKKO_HPP
#define SMC_SCRIPTING_ROKKO_HPP
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcRokko;
		void Init_Rokko(mrb_state* p_state);
	}
}
#endif
