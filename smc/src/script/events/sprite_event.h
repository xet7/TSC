// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPT_SPRITE_EVENT_H
#define SMC_SCRIPT_SPRITE_EVENT_H
#include <string>
#include "../script.h"
#include "../../objects/sprite.h"
#include "event.h"

namespace SMC{
	namespace Script{
		class cSprite_Event: public cEvent
		{
		public:
			virtual std::string Event_Name();
			void Fire(cLua_Interpreter* p_lua, cSprite* p_sprite);
		protected:
			virtual int Run_Lua_Callback(cLua_Interpreter* p_lua);
		};
	};
};

#endif
