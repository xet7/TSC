// -*- c++ -*-
#ifndef SMC_SCRIPT_DIE_H
#define SMC_SCRIPT_DIE_H
#include <string>
#include "../luawrap.hpp"
#include "sprite_event.h"
namespace SMC{
  namespace Script{
    class cDie_Event: public cSprite_Event
    {
    public:
      virtual std::string Event_Name(){return "die";}
    };
  };
};
#endif
