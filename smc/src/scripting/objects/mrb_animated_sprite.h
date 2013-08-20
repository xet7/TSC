// -*- c++ -*-
#ifndef SMC_SCRIPTING_MRB_ANIMATED_SPRITE_H
#define SMC_SCRIPTING_MRB_ANIMATED_SPRITE_H

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcAnimated_Sprite;
		extern struct mrb_data_type rtAnimated_Sprite;

		void Init_Animated_Sprite(mrb_state* p_state);
	};
};

#endif
