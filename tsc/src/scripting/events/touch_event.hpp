#ifndef SMC_SCRIPTING_TOUCH_EVENT_HPP
#define SMC_SCRIPTING_TOUCH_EVENT_HPP
#include "../scripting.hpp"
#include "event.hpp"

namespace SMC {
    namespace Scripting {
        class cTouch_Event: public cEvent {
        public:
            cTouch_Event(cSprite* p_collided);
            virtual std::string Event_Name();
            cSprite* Get_Collided();
        protected:
            virtual void Run_MRuby_Callback(cMRuby_Interpreter* p_mruby, mrb_value callback);
        private:
            cSprite* mp_collided;
        };
    }
}
#endif
