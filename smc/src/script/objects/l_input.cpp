// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../script.h"
#include "../events/key_down_event.h"
#include "../../input/keyboard.h"
#include "../../input/joystick.h"
#include "l_input.h"

using namespace SMC;

/***************************************
 * Events
 ***************************************/

LUA_IMPLEMENT_EVENT(key_down);

/***************************************
 * Binding
 ***************************************/

static luaL_Reg Methods[] = {
	{"on_key_down", LUA_EVENT_HANDLER(key_down)},
	{NULL, NULL}
};

void Script::Open_Input(lua_State* p_state)
{
	LuaWrap::register_subclass<cKeyboard>(	p_state, // Note that the template parameter is ignored as I don’t use LuaWrap’s automatic memory management
											"InputClass",
											"Object",
											Methods,
											NULL, // Singleton, can’t be instanciated
											NULL); // Memory managed by SMC

	// Make the global variable `Input' point to the sole instance
	// of the InputClass class.
	lua_getglobal(p_state, "InputClass"); // Class needed for the instance method table
	cKeyboard** ptr = (cKeyboard**) lua_newuserdata(p_state, sizeof(cKeyboard*));
	*ptr = pKeyboard;
	LuaWrap::InternalC::set_imethod_table(p_state, -2); // Attach instance methods
	// Cleanup the stack, remove the class table
	lua_insert(p_state, -2);
	lua_pop(p_state, 1);
	// Make the global variable
	lua_setglobal(p_state, "Input");
}
