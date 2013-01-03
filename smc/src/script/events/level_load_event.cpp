// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../../level/level.h"
#include "level_load_event.h"

using namespace SMC;

Script::cLevel_Load_Event::cLevel_Load_Event(Lua_Save_Data* p_data)
{
	mp_data = p_data;
}

std::string Script::cLevel_Load_Event::Event_Name()
{
	return "load";
}

Lua_Save_Data* Script::cLevel_Load_Event::Get_Save_Data()
{
	return mp_data;
}

int Script::cLevel_Load_Event::Run_Lua_Callback(cLua_Interpreter* p_lua)
{
	lua_State* p_state = p_lua->Get_Lua_State();

	// Create a new and empty Lua table
	Lua_Save_Data::iterator iter;
	lua_newtable(p_state);

	// Convert the C++ data hashtable by filling the
	// Lua table with its keys and values.
	for(iter = mp_data->begin(); iter != mp_data->end(); iter++){
		lua_pushstring(p_state, (*iter).first.c_str());		// Key
		lua_pushstring(p_state, (*iter).second.c_str());	// Value
		lua_settable(p_state, -3);
	}

	// Hand the constructed Lua table to the callback.
	return lua_pcall(p_state, 1, 0, 0);
}
