// -*- c++ -*-
#ifndef SMC_SCRIPTING_MUSHROOM_H
#define SMC_SCRIPTING_MUSHROOM_H
#include "../../scripting.h"

namespace SMC {
  namespace Scripting {
    extern struct RClass* p_rcMushroom;
    void Init_Mushroom(mrb_state* p_state);
  }
}
#endif
