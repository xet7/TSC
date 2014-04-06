// -*- c++ -*-
#ifndef SMC_SCRIPTING_TEXTBOX_H
#define SMC_SCRIPTING_TEXTBOX_H
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcText_Box;
		void Init_TextBox(mrb_state* p_state);
	}
}
#endif
