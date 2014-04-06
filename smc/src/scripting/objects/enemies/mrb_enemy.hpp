// -*- c++ -*-
#ifndef SMC_SCRIPTING_ENEMY_H
#define SMC_SCRIPTING_ENEMY_H
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcEnemy;
		void Init_Enemy(mrb_state* p_state);
	}
}
#endif
