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

static int Register_Key_Down_Event(lua_State* p_state)
{
	if (!lua_isuserdata(p_state, 1))
		return luaL_error(p_state, "Argument #1 is not a userdata.");
	if (!lua_isfunction(p_state, 2))
		return luaL_error(p_state, "Argument #2 is not a function.");

	// Save the handler function into the registry
	lua_pushvalue(p_state, 2); // Don’t remove the argument (keep the stack balanced)
	int ref = luaL_ref(p_state, LUA_REGISTRYINDEX);

	// Store the reference to the function for later calling
	Script::cKeyDownEvent::Register_Handler(ref);

	return 0;
}

/***************************************
 * Binding
 ***************************************/

static luaL_Reg Methods[] = {
	{"on_key_down", Register_Key_Down_Event},
	{NULL, NULL}
};

void Script::Open_Input(lua_State* p_state)
{
	LuaWrap::register_class<cKeyboard>(	p_state, // Note that the template parameter is ignored as I don’t use LuaWrap’s automatic memory management
										"InputClass",
										Methods,
										NULL, // Singleton, can’t be instanciated
										NULL); // Memory managed by SMC

	// Make the global variable `Input' point to the sole instance
	// of the InputClass class.
	lua_getglobal(p_state, "InputClass"); // Class needed for the instance method table
	void** ptr = (void**) lua_newuserdata(p_state, sizeof(void*));
	*ptr = NULL; // Empty userdata object, this pointer is not used
	LuaWrap::InternalC::set_imethod_table(p_state, -2); // Attach instance methods
	// Cleanup the stack, remove the class table
	lua_insert(p_state, -2);
	lua_pop(p_state, 1);
	// Make the global variable
	lua_setglobal(p_state, "Input");
}
