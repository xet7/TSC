// -*- c++ -*-
#ifndef SMC_SCRIPTING_POWERUP_H
#define SMC_SCRIPTING_POWERUP_H
#include "../../scripting.h"

namespace SMC {
  namespace Scripting {
    extern struct RClass* p_rcPowerup;
    void Init_Powerup(mrb_state* p_state);
  }
}
#endif
