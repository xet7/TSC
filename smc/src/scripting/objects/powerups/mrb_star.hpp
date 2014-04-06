#ifndef SMC_SCRIPTING_STAR_H
#define SMC_SCRIPTING_STAR_H
#include "../../scripting.hpp"

namespace SMC {
  namespace Scripting {
    extern struct RClass* p_rcStar;
    void Init_Star(mrb_state* p_state);
  }
}
#endif
