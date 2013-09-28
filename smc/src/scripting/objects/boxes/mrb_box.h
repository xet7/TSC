// -*- c++ -*-
#ifndef SMC_SCRIPTING_BOX_H
#define SMC_SCRIPTING_BOX_H
#include "../../scripting.h"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcBox;
		void Init_Box(mrb_state* p_state);
	}
}
#endif
