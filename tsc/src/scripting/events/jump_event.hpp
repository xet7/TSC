#ifndef TSC_SCRIPTING_JUMP_EVENT_HPP
#define TSC_SCRIPTING_JUMP_EVENT_HPP
#include "../scripting.hpp"
#include "event.hpp"

namespace TSC {
    namespace Scripting {

        class cJump_Event: public cEvent {
        public:
            virtual std::string Event_Name()
            {
                return "jump";
            }
        };
    }
}

#endif
