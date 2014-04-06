#ifndef SMC_SCRIPTING_AUDIO_HPP
#define SMC_SCRIPTING_AUDIO_HPP
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcAudio;
		void Init_Audio(mrb_state* p_state);
	}
}

#endif
