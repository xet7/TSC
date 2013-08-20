// -*- c++ -*-
#ifndef SMC_SCRIPTING_ENEMY_H
#define SMC_SCRIPTING_ENEMY_H
#include "../scripting.h"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcEnemy;
		extern struct mrb_data_type rtEnemy;
		void Init_Enemy(mrb_state* p_state);
	}
}
#endif
