// -*- c++ -*-
#ifndef SMC_SCRIPTING_BONUSBOX_H
#define SMC_SCRIPTING_BONUSBOX_H
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcBonus_Box;
		void Init_BonusBox(mrb_state* p_state);
	}
}
#endif
