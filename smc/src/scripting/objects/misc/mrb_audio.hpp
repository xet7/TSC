// -*- c++ -*-
#ifndef SMC_SCRIPTING_AUDIO_H
#define SMC_SCRIPTING_AUDIO_H
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcAudio;
		void Init_Audio(mrb_state* p_state);
	}
}

#endif
