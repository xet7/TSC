#ifndef SMC_SCRIPTING_LEVEL_HPP
#define SMC_SCRIPTING_LEVEL_HPP
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcLevel;
		void Init_Level(mrb_state* p_state);
	}
}

#endif
