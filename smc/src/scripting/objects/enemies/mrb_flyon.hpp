// -*- c++ -*-
#ifndef SMC_SCRIPTING_FLYON_H
#define SMC_SCRIPTING_FLYON_H
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcFlyon;
		void Init_Flyon(mrb_state* p_state);
	}
}
#endif
