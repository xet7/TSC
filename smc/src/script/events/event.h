// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPT_EVENTS_EVENT_H
#define SMC_SCRIPT_EVENTS_EVENT_H
#include <string>
#include <vector>
#include "../script.h"
#include "../scriptable_object.h"

// Defines a event handler function that just calls
// Forward_To_Register with the given argument.
#define LUA_IMPLEMENT_EVENT(evtname)\
	static int Script_Event_On_##evtname(lua_State* p_state){return SMC::Script::Forward_To_Register(p_state, #evtname);}
// Expands to the name of the function defined
// by IMPLEMENT_LUA_EVENT.
#define LUA_EVENT_HANDLER(evtname) Script_Event_On_##evtname


namespace SMC{
	namespace Script{
		class cEvent
		{
		public:
			void Fire(cLua_Interpreter* p_lua, cScriptable_Object* p_obj);
			virtual std::string Event_Name();
		protected:
			virtual int Run_Lua_Callback(cLua_Interpreter* p_lua);
		};

		int Forward_To_Register(lua_State* p_state, std::string event_name);
	};
};
#endif
