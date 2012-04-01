// -*- c++ -*-
#ifndef SMC_SCRIPT_SPRITE_H
#define SMC_SCRIPT_SPRITE_H
#include <string>
#include "../luawrap.hpp"
// Defines a event handler function that just calls
// Forward_To_Register with the given argument.
#define LUA_IMPLEMENT_EVENT(evtname)\
	static int Lua_Sprite_On_##evtname(lua_State* p_state){return SMC::Script::Forward_To_Sprite_Register(p_state, #evtname);}
// Expands to the name of the function defined
// by IMPLEMENT_LUA_EVENT.
#define LUA_EVENT_HANDLER(evtname) Lua_Sprite_On_##evtname

namespace SMC{
	namespace Script{
		int Forward_To_Sprite_Register(lua_State* p_state, std::string event_name);
		void Open_Sprite(lua_State* p_state);
	};
};

#endif
