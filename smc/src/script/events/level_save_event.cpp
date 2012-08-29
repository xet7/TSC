// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../../level/level.h"
#include "level_save_event.h"

using namespace SMC;

Script::cLevel_Save_Event::cLevel_Save_Event(Lua_Save_Data* p_data)
{
	mp_data = p_data;
}

std::string Script::cLevel_Save_Event::Event_Name()
{
	return "save";
}

Lua_Save_Data* Script::cLevel_Save_Event::Get_Save_Data()
{
	return mp_data;
}

int Script::cLevel_Save_Event::Run_Lua_Callback(cLua_Interpreter* p_lua)
{
	lua_State* p_state = p_lua->Get_Lua_State();

	// The callback is supposed to place a table onto
	// the lua stack.
	int result = lua_pcall(p_state, 0, 1, 0);

	// If the callback failed, propagate the error
	if (result != LUA_OK)
		return result;
	// Crash if no table was given (mimicing pcall’s behaviour
	// for the error handler in cEvent::Fire()).
	if (!lua_istable(p_state, -1)){
		lua_pushstring(p_state, "Callback for `save' did not return a table!");
		return LUA_ERRRUN;
	}

	// Otherwise, translate the Lua table into a C++ one.
	lua_pushnil(p_state); // First key
	while (lua_next(p_state, -2)){
		/* Check if we *really* got a string key. lua_isstring() returns
		 * true for numbers as well, but we are not allowed to call
		 * lua_tostring within a lua_next() loop with a non-string key. Doing
		 * this on the value is not a problem. See the Lua documentation
		 * of lua_next(). */
		if (lua_isstring(p_state, -2) && !(lua_isnumber(p_state, -2)))
			(*mp_data)[lua_tostring(p_state, -2)] = luaL_checkstring(p_state, -1);

		// Remove the value from the stack,
		// keep the key for lua_next()
		lua_pop(p_state, 1);
	}

	return result;
}
