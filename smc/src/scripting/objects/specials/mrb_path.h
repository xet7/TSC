// -*- c++ -*-
#ifndef SMC_SCRIPTING_PATH_H
#define SMC_SCRIPTING_PATH_H
#include "../../scripting.h"

namespace SMC {
  namespace Scripting {
    extern struct RClass* p_rcPath;
    void Init_Path(mrb_state* p_state);
  }
}
#endif
