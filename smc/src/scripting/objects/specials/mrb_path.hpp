#ifndef SMC_SCRIPTING_PATH_HPP
#define SMC_SCRIPTING_PATH_HPP
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcPath;
		extern struct RClass* p_rcPath_Segment;
		extern struct mrb_data_type rtSMC_Path_Segment;
		void Init_Path(mrb_state* p_state);
	}
}
#endif
