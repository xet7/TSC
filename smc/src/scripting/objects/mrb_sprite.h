// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPTING_MRB_SPRITE_H
#define SMC_SCRIPTING_MRB_SPRITE_H
#include "../scripting.h"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcSprite;
		extern struct mrb_data_type rtSprite;

		void Init_Sprite(mrb_state* p_state);
	}
}
#endif
