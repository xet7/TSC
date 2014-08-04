#ifndef SMC_SCRIPTING_KEY_DOWN_EVENT_HPP
#define SMC_SCRIPTING_KEY_DOWN_EVENT_HPP
#include "../scripting.hpp"
#include "event.hpp"

namespace SMC {
	namespace Scripting {
		class cKeyDown_Event: public cEvent
		{
		public:
			cKeyDown_Event(string keyname);
			virtual string Event_Name();
			string Get_Keyname();
		protected:
			virtual void Run_MRuby_Callback(cMRuby_Interpreter* p_mruby, mrb_value callback);
		private:
			string m_keyname;
		};
	}
}
#endif
