#ifndef SMC_SCRIPTING_SMC_HPP
#define SMC_SCRIPTING_SMC_HPP
#include "../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rmSMC;

		void Init_SMC(mrb_state* p_state);
	}
}

#endif
