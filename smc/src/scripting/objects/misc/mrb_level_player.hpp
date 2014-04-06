// -*- c++ -*-
#ifndef SMC_SCRIPTING_LEVEL_PLAYER_H
#define SMC_SCRIPTING_LEVEL_PLAYER_H
#include "../../scripting.h"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcLevel_Player;
		void Init_Level_Player(mrb_state* p_state);
	};
};

#endif
