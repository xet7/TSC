// -*- c++ -*-
#ifndef SMC_SCRIPTING_SMC_H
#define SMC_SCRIPTING_SMC_H
#include "../scripting.h"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rmSMC;

		void Init_SMC(mrb_state* p_state);
	}
}

#endif
