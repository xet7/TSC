// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPTING_LEVEL_PLAYER_H
#define SMC_SCRIPTING_LEVEL_PLAYER_H
#include "../scripting.h"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcLevel_Player;
		extern struct mrb_data_type rtLevel_Player;
		void Init_Level_Player(mrb_state* p_state);
	};
};

#endif
