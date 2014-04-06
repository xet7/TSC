#ifndef SMC_SCRIPTING_STATIC_HPP
#define SMC_SCRIPTING_STATIC_HPP
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcStaticEnemy;
		void Init_StaticEnemy(mrb_state* p_state);
	}
}
#endif
