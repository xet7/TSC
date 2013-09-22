// -*- c++ -*-
#ifndef SMC_SCRIPTING_MRB_SPRITE_H
#define SMC_SCRIPTING_MRB_SPRITE_H
#include "../../scripting.h"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcSprite;

		void Init_Sprite(mrb_state* p_state);
	}
}
#endif
