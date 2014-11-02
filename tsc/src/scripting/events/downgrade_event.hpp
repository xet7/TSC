#ifndef TSC_SCRIPTING_DOWNGRADE_EVENT_HPP
#define TSC_SCRIPTING_DOWNGRADE_EVENT_HPP
#include "../scripting.hpp"
#include "event.hpp"

namespace TSC {
    namespace Scripting {
        class cDowngrade_Event: public cEvent {
        public:
            cDowngrade_Event(int downgrades, int max_downgrades);
            virtual std::string Event_Name();
            int Get_Downgrades();
            int Get_Max_Downgrades();
        protected:
            virtual void Run_MRuby_Callback(cMRuby_Interpreter* p_mruby, mrb_value callback);
        private:
            int m_downgrades;
            int m_max_downgrades;
        };
    }
}
#endif
