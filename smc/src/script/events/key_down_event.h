// -*- c++ -*-
#ifndef SMC_SCRIPT_EVENTS_KEY_DOWN
#define SMC_SCRIPT_EVENTS_KEY_DOWN
#include "../luawrap.hpp"
#include "event.h"

namespace SMC{
	namespace Script{
		class cKeyDownEvent: public cEvent
		{
		public:
			cKeyDownEvent(std::string keyname);
			std::string Get_Keyname();
		private:
			virtual int Run_Lua_Callback(lua_State* p_state);
			std::string m_keyname;
		};

		void Open_Key_Down_Event(lua_State* p_state);
	};
};


#endif
