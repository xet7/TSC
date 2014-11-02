#ifndef SMC_SCRIPTING_DIE_EVENT_HPP
#define SMC_SCRIPTING_DIE_EVENT_HPP
#include "../scripting.hpp"
#include "event.hpp"

namespace TSC {
    namespace Scripting {

        class cDie_Event: public cEvent {
        public:
            virtual std::string Event_Name()
            {
                return "die";
            }
        };
    }
}

#endif
