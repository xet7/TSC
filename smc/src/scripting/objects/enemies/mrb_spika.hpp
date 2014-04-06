#ifndef SMC_SCRIPTING_SPIKA_H
#define SMC_SCRIPTING_SPIKA_H
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcSpika;
		void Init_Spika(mrb_state* p_state);
	}
}
#endif
