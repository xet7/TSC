// -*- c++ -*-
#ifndef SMC_SCRIPT_DOWNGRADE_EVENT_H
#define SMC_SCRIPT_DOWNGRADE_EVENT_H
#include <string>
#include "../luawrap.hpp"
#include "sprite_event.h"

namespace SMC{
	namespace Script{
		class cDowngrade_Event: public cSprite_Event
		{
		public:
			cDowngrade_Event(int downgrades, int max_downgrades);
			virtual std::string Event_Name();
			int Get_Downgrades();
			int Get_Max_Downgrades();
		protected:
			virtual int Run_Lua_Callback(lua_State* p_state);
		private:
			int m_downgrades;
			int m_max_downgrades;
		};
	};
};
#endif
