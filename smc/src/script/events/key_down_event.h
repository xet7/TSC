// -*- c++ -*-
#ifndef SMC_SCRIPT_EVENTS_KEY_DOWN
#define SMC_SCRIPT_EVENTS_KEY_DOWN
#include "../luawrap.hpp"
#include "event.h"

namespace SMC{
	namespace Script{

    /**
     * Key down event. This is triggered every time the player
     * presses a key recognized by SMC, i.e. you cannot listen
     * for keys not listed below.
     *
     * The list of keys this event is triggered for is as follows:
     *
     * * shoot
     * * jump
     * * action
     * * up
     * * down
     * * left
     * * right
     * * item
     *
     * The event handler gets passed a string representing the
     * pressed key (see list above).
     */
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
