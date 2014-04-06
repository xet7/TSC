#ifndef SMC_SCRIPTING_STAR_HPP
#define SMC_SCRIPTING_STAR_HPP
#include "../../scripting.hpp"

namespace SMC {
  namespace Scripting {
    extern struct RClass* p_rcStar;
    void Init_Star(mrb_state* p_state);
  }
}
#endif
