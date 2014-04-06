#ifndef SMC_SCRIPTING_MOON_H
#define SMC_SCRIPTING_MOON_H
#include "../../scripting.hpp"

namespace SMC {
  namespace Scripting {
    extern struct RClass* p_rcMoon;
    void Init_Moon(mrb_state* p_state);
  }
}
#endif
