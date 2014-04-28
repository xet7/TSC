#ifndef SMC_SCRIPTING_EVENTABLE_HPP
#define SMC_SCRIPTING_EVENTABLE_HPP
#include "../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rmEventable;

		void Init_Eventable(mrb_state* p_state);
	};
};

#endif
