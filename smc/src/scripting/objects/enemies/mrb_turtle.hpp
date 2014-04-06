#ifndef SMC_SCRIPTING_TURTLE_HPP
#define SMC_SCRIPTING_TURTLE_HPP
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcTurtle;
		void Init_Turtle(mrb_state* p_state);
	}
}
#endif
