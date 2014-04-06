// -*- c++ -*-
#ifndef SMC_SCRIPTING_EATO_H
#define SMC_SCRIPTING_EATO_H
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcEato;
		void Init_Eato(mrb_state* p_state);
	}
}
#endif
