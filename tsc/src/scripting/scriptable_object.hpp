#ifndef TSC_SCRIPTING_SCRIPTABLE_OBJECT_HPP
#define TSC_SCRIPTING_SCRIPTABLE_OBJECT_HPP
#include "../core/global_basic.hpp"

namespace TSC {
    namespace Scripting {

        /**
         * This class encapsulates the stuff that is common
         * to all objects exposed to the mruby scripting
         * interface. That is, it holds the mruby event tables.
         */
        class cScriptable_Object {
        public:
            cScriptable_Object();
            virtual ~cScriptable_Object();

            void clear_event_handlers();
            void register_event_handler(const std::string& evtname, mrb_value callback);
            std::vector<mrb_value>::iterator event_handlers_begin(const std::string& evtname);
            std::vector<mrb_value>::iterator event_handlers_end(const std::string& evtname);

        protected:
            /// Mapping of event names and registered callbacks.
            std::map<std::string, std::vector<mrb_value> > m_callbacks;
        };
    };
};
#endif
