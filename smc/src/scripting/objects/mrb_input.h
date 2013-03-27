// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPTING_INPUT_H
#define SMC_SCRIPTING_INPUT_H
#include "../scripting.h"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcInput;
		extern struct mrb_data_type rtInput;
		void Init_Input(mrb_state* p_state);
	}
}

#endif
