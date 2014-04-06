#ifndef SMC_SCRIPTING_EATO_HPP
#define SMC_SCRIPTING_EATO_HPP
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcEato;
		void Init_Eato(mrb_state* p_state);
	}
}
#endif
