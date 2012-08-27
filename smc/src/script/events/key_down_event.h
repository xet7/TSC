// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPT_EVENTS_KEY_DOWN
#define SMC_SCRIPT_EVENTS_KEY_DOWN
#include "../script.h"
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
			virtual std::string Event_Name();
		private:
			virtual int Run_Lua_Callback(cLua_Interpreter* p_lua);
			std::string m_keyname;
		};
	};
};


#endif
