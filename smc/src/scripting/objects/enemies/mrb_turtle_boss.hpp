#ifndef SMC_SCRIPTING_TURTLE_BOSS_HPP
#define SMC_SCRIPTING_TURTLE_BOSS_HPP
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcTurtleBoss;
		void Init_TurtleBoss(mrb_state* p_state);
	}
}
#endif
