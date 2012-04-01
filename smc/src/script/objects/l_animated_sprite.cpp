#include "../luawrap.hpp"
#include "../../objects/animated_sprite.h"
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

static luaL_Reg Animated_Sprite_Methods[] = {
	{NULL, NULL}
};

void Script::Open_Animated_Sprite(lua_State* p_state)
{
	LuaWrap::register_subclass<cAnimated_Sprite>(p_state,
	                                            "AnimatedSprite",
	                                            "MovingSprite",
	                                            Animated_Sprite_Methods,
	                                            NULL,
	                                            NULL,  // Not intended to be instanciated directly
	                                            NULL); // Memory managed by SMC
}
