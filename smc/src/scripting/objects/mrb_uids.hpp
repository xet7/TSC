#ifndef SMC_SCRIPTING_UIDS_HPP
#define SMC_SCRIPTING_UIDS_HPP
#include "../scripting.hpp"

namespace SMC {
	namespace Scripting {
		void Init_UIDS(mrb_state* p_state);
		void Delete_UID_From_Cache(mrb_state* p_state, int uid);
	}
}

#endif
