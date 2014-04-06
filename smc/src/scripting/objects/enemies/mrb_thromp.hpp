// -*- c++ -*-
#ifndef SMC_SCRIPTING_THROMP_H
#define SMC_SCRIPTING_THROMP_H
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcThromp;
		void Init_Thromp(mrb_state* p_state);
	}
}
#endif
