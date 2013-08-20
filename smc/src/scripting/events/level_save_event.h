// -*- c++ -*-
#ifndef SMC_SCRIPTING_LEVEL_SAVE_EVENT_H
#define SMC_SCRIPTING_LEVEL_SAVE_EVENT_H
#include "../scripting.h"
#include "event.h"

namespace SMC {
	namespace Scripting {
		class cLevel_Save_Event: public cEvent
		{
		public:
			virtual std::string Event_Name();
			std::string Get_Save_Data();
		protected:
			virtual void Run_MRuby_Callback(cMRuby_Interpreter* p_mruby, mrb_value callback);
		private:
			std::string m_save_data;
		};
	}
}
#endif
