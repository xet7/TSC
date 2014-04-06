// -*- c++ -*-
#ifndef SMC_SCRIPTING_KEY_DOWN_EVENT_H
#define SMC_SCRIPTING_KEY_DOWN_EVENT_H
#include "../scripting.hpp"
#include "event.hpp"

namespace SMC {
	namespace Scripting {
		class cKeyDown_Event: public cEvent
		{
		public:
			cKeyDown_Event(std::string keyname);
			virtual std::string Event_Name();
			std::string Get_Keyname();
		protected:
			virtual void Run_MRuby_Callback(cMRuby_Interpreter* p_mruby, mrb_value callback);
		private:
			std::string m_keyname;
		};
	}
}
#endif
