// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../script.h"
#include "../scriptable_object.h"
#include "../../level/level_player.h"
#include "l_object.h"

/***************************************
 * Methods
 ***************************************/
using namespace SMC;

static int Allocate(lua_State* p_state)
{

	if (!lua_istable(p_state, 1))
		return luaL_error(p_state, "No class table given.");

	Script::cScriptable_Object** pp_obj	= (Script::cScriptable_Object**) lua_newuserdata(p_state, sizeof(Script::cScriptable_Object*));
	*pp_obj						= new Script::cScriptable_Object;

	LuaWrap::InternalC::set_imethod_table(p_state, 1); // Attach instance methods

	return 1;
}

static int Finalize(lua_State* p_state)
{
	Script::cScriptable_Object* p_obj = *LuaWrap::check<Script::cScriptable_Object*>(p_state, 1);
	delete p_obj;
	return 0;
}

/**
 * Generic event handler registration. Takes the name of
 * the event you want to register for and the Lua function
 * to register. Call from Lua like this:
 *
 * your_sprite:register("touch", yourfunction)
 *
 * Note you usually don’t want to call this directly,
 * but rather use something along the lines of on_touch().
 */
static int Register(lua_State* p_state)
{
	Script::cScriptable_Object* p_obj = *LuaWrap::check<Script::cScriptable_Object*>(p_state, 1);
	const char* str = luaL_checkstring(p_state, 2);
	if (!lua_isfunction(p_state, 3))
		return luaL_error(p_state, "No function given.");

	// Copy the function to the Lua registry
	lua_pushvalue(p_state, 3); // Don’t remove the argument (keep the stack balanced)
	int ref = luaL_ref(p_state, LUA_REGISTRYINDEX);

	// Add the event handler to the list (if the request event key
	// doesn’t yet exist, it will automatically be created).
	std::cout << str << std::endl;
	p_obj->m_event_table[str].push_back(ref);

	return 0;
}

/***************************************
 * Binding
 ***************************************/

static luaL_Reg Methods[] = {
	{"register", Register},
	{NULL, NULL}
};

void SMC::Script::Open_Object(lua_State* p_state)
{
	LuaWrap::register_class<Script::cScriptable_Object>(	p_state,
															"Object",
															Methods,
															NULL,
															Allocate,
															Finalize);
}
