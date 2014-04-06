// -*- c++ -*-
#ifndef SMC_SCRIPTING_LEVEL_LOAD_EVENT_H
#define SMC_SCRIPTING_LEVEL_LOAD_EVENT_H
#include "../scripting.hpp"
#include "event.hpp"

namespace SMC {
	namespace Scripting {
		class cLevel_Load_Event: public cEvent
		{
		public:
			cLevel_Load_Event(std::string save_data);
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
