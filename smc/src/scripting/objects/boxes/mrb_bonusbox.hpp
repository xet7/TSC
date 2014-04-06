#ifndef SMC_SCRIPTING_BONUSBOX_HPP
#define SMC_SCRIPTING_BONUSBOX_HPP
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcBonus_Box;
		void Init_BonusBox(mrb_state* p_state);
	}
}
#endif
