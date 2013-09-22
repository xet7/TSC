// -*- c++ -*-
#ifndef SMC_SCRIPTING_LEVEL_H
#define SMC_SCRIPTING_LEVEL_H
#include "../../scripting.h"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcLevel;
		void Init_Level(mrb_state* p_state);
	}
}

#endif
