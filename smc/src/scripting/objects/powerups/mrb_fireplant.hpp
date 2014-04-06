#ifndef SMC_SCRIPTING_FIREPLANT_HPP
#define SMC_SCRIPTING_FIREPLANT_HPP
#include "../../scripting.hpp"

namespace SMC {
  namespace Scripting {
    extern struct RClass* p_rcFireplant;
    void Init_Fireplant(mrb_state* p_state);
  }
}
#endif
