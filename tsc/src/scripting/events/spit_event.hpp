#ifndef TSC_SCRIPTING_SPIT_EVENT_HPP
#define TSC_SCRIPTING_SPIT_EVENT_HPP
#include "../scripting.hpp"
#include "event.hpp"

namespace TSC {
    namespace Scripting {
        class cSpit_Event: public cEvent {
        public:
            virtual std::string Event_Name()
            {
                return "spit";
            }
        };
    }
}
#endif
