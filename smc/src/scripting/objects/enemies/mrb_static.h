// -*- c++ -*-
#ifndef SMC_SCRIPTING_STATIC_H
#define SMC_SCRIPTING_STATIC_H
#include "../../scripting.h"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcStaticEnemy;
		void Init_StaticEnemy(mrb_state* p_state);
	}
}
#endif
