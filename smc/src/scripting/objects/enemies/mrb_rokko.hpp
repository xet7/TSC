// -*- c++ -*-
#ifndef SMC_SCRIPTING_ROKKO_H
#define SMC_SCRIPTING_ROKKO_H
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcRokko;
		void Init_Rokko(mrb_state* p_state);
	}
}
#endif
