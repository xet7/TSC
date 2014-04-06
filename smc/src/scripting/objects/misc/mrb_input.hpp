// -*- c++ -*-
#ifndef SMC_SCRIPTING_INPUT_H
#define SMC_SCRIPTING_INPUT_H
#include "../../scripting.h"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcInput;
		void Init_Input(mrb_state* p_state);
	}
}

#endif
