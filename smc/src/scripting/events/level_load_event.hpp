#ifndef SMC_SCRIPTING_LEVEL_LOAD_EVENT_HPP
#define SMC_SCRIPTING_LEVEL_LOAD_EVENT_HPP
#include "../scripting.hpp"
#include "event.hpp"

namespace SMC {
	namespace Scripting {
		class cLevel_Load_Event: public cEvent
		{
		public:
			cLevel_Load_Event(string save_data);
			virtual string Event_Name();
			string Get_Save_Data();
		protected:
			virtual void Run_MRuby_Callback(cMRuby_Interpreter* p_mruby, mrb_value callback);
		private:
			string m_save_data;
		};
	}
}
#endif
