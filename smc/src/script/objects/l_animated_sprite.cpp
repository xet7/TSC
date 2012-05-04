// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../script.h"
#include "l_sprite.h"
#include "l_animated_sprite.h"

using namespace SMC;

/***************************************
 * Event handlers
 ***************************************/

/***************************************
 * "Normal" access
 ***************************************/

/***************************************
 * Binding
 ***************************************/

static luaL_Reg Methods[] = {
	{NULL, NULL}
};

void Script::Open_Animated_Sprite(lua_State* p_state)
{
	LuaWrap::register_subclass<cAnimated_Sprite>(	p_state,
													"AnimatedSprite",
													"MovingSprite",
													Methods,
													NULL,
													NULL,	// Not intended to be instanciated directly
													NULL);	// Memory managed by SMC
}
