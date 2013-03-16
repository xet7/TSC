// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPTING_SMC_H
#define SMC_SCRIPTING_SMC_H
#include <map>
#include "../scripting.h"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rmSMC;

		void Init_SMC(mrb_state* p_state);
	}
}

#endif
