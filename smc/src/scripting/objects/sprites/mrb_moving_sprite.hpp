#ifndef SMC_SCRIPTING_MRB_MOVING_SPRITE_H
#define SMC_SCRIPTING_MRB_MOVING_SPRITE_H

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcMoving_Sprite;

		void Init_Moving_Sprite(mrb_state* p_state);
	};
};

#endif