// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPTING_EATO_H
#define SMC_SCRIPTING_EATO_H
#include "../scripting.h"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcEato;
		extern struct mrb_data_type rtEato;
		void Init_Eato(mrb_state* p_state);
	}
}
#endif
