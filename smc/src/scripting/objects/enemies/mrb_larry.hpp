#ifndef SMC_SCRIPTING_LARRY_HPP
#define SMC_SCRIPTING_LARRY_HPP
#include "../../scripting.hpp"
namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcLarry;
		void Init_Larry(mrb_state* p_state);
	}
}
#endif
