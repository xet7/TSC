// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPTING_LEVEL_H
#define SMC_SCRIPTING_LEVEL_H
#include "../scripting.h"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcLevel;
		extern struct mrb_data_type rtLevel;
		void Init_Level(mrb_state* p_state);
	}
}

#endif
