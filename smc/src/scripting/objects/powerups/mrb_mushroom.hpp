#ifndef SMC_SCRIPTING_MUSHROOM_HPP
#define SMC_SCRIPTING_MUSHROOM_HPP
#include "../../scripting.hpp"

namespace SMC {
  namespace Scripting {
    extern struct RClass* p_rcMushroom;
    void Init_Mushroom(mrb_state* p_state);
  }
}
#endif
