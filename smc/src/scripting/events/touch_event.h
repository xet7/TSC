// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPTING_TOUCH_EVENT_H
#define SMC_SCRIPTING_TOUCH_EVENT_H
#include "../scripting.h"
#include "event.h"

namespace SMC {
	namespace Scripting {
		class cTouch_Event: public cEvent
		{
		public:
			cTouch_Event(cSprite* p_collided);
			virtual std::string Event_Name();
			cSprite* Get_Collided();
		protected:
			virtual void Run_MRuby_Callback(cMRuby_Interpreter* p_mruby, mrb_value callback);
		private:
			cSprite* mp_collided;
		};
	}
}
#endif
