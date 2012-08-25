// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPT_DOWNGRADE_EVENT_H
#define SMC_SCRIPT_DOWNGRADE_EVENT_H
#include <string>
#include "../script.h"
#include "event.h"

namespace SMC{
	namespace Script{
		class cDowngrade_Event: public cEvent
		{
		public:
			cDowngrade_Event(int downgrades, int max_downgrades);
			virtual std::string Event_Name();
			int Get_Downgrades();
			int Get_Max_Downgrades();
		protected:
			virtual int Run_Lua_Callback(cLua_Interpreter* p_lua);
		private:
			int m_downgrades;
			int m_max_downgrades;
		};
	};
};
#endif
