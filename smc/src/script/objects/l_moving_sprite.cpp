// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../luawrap.hpp"
#include "../../objects/movingsprite.h"
#include "l_sprite.h"
#include "l_moving_sprite.h"

using namespace SMC;

/***************************************
 * Event handlers
 ***************************************/

/***************************************
 * "Normal" access
 ***************************************/

static int Get_Direction(lua_State* p_state)
{
	cMovingSprite* p_msprite = *LuaWrap::check<cMovingSprite*>(p_state, 1);
	std::string dir;
	switch(p_msprite->m_direction){
	case DIR_UNDEFINED:
		dir = "undefined";
		break;
	case DIR_LEFT:
		dir = "left";
		break;
	case DIR_RIGHT:
		dir = "right";
		break;
	case DIR_UP:
		dir = "up";
		break;
	case DIR_DOWN:
		dir = "down";
		break;
	case DIR_UP_LEFT:
		dir = "up_left";
		break;
	case DIR_UP_RIGHT:
		dir = "up_right";
		break;
	case DIR_DOWN_LEFT:
		dir = "down_left";
		break;
	case DIR_DOWN_RIGHT:
		dir = "down_right";
		break;
	case DIR_LEFT_UP:
		dir = "left_up";
		break;
	case DIR_LEFT_DOWN:
		dir = "left_down";
		break;
	case DIR_HORIZONTAL:
		dir = "horizontal";
		break;
	case DIR_VERTICAL:
		dir = "vertical";
		break;
	case DIR_ALL:
		dir = "all";
		break;
	case DIR_FIRST:
		dir = "first";
		break;
	case DIR_LAST:
		dir = "last";
		break;
	default: // Shouldn’t happen
		dir = "invalid";
		break;
	}
	// A world for a consecutive enum!

	lua_pushstring(p_state, dir.c_str());
	return 1;
}

static int Set_Direction(lua_State* p_state)
{
	cMovingSprite* p_msprite = *LuaWrap::check<cMovingSprite*>(p_state, 1);
	std::string dirstr = luaL_checkstring(p_state, 2);
	ObjectDirection dir;
	if (dirstr == "left")
		dir = DIR_LEFT;
	else if (dirstr == "right")
		dir = DIR_RIGHT;
	else if (dirstr == "up" || dirstr == "top")
		dir = DIR_UP;
	else if (dirstr == "down" || dirstr == "bottom")
		dir = DIR_DOWN;
	else if (dirstr == "up_left" || dirstr == "top_left")
		dir = DIR_UP_LEFT;
	else if (dirstr == "up_right" || dirstr == "top_right")
		dir = DIR_UP_RIGHT;
	else if (dirstr == "down_left" || dirstr == "bottom_left")
		dir = DIR_DOWN_LEFT;
	else if (dirstr == "down right" || dirstr == "bottom_right")
		dir = DIR_DOWN_RIGHT;
	else if (dirstr == "left_up" || dirstr == "left_top")
		dir = DIR_LEFT_UP;
	else if (dirstr == "left_down" || dirstr == "left_bottom")
		dir = DIR_LEFT_DOWN;
	else if (dirstr == "right_up" || dirstr == "right_top")
		dir = DIR_RIGHT_UP;
	else if (dirstr == "right_down" || dirstr == "right_bottom")
		dir = DIR_RIGHT_DOWN;
	else if (dirstr == "horizontal")
		dir = DIR_HORIZONTAL;
	else if (dirstr == "vertical")
		dir = DIR_VERTICAL;
	else if (dirstr == "all")
		dir = DIR_ALL;
	else if (dirstr == "first")
		dir = DIR_FIRST;
	else if (dirstr == "last")
		dir = DIR_LAST;
	else
		return luaL_error(p_state, "Invalid direction '%s'.", dirstr.c_str());
	// A world for a consecutive enum!

	p_msprite->Set_Direction(dir);

	return 0;
}

static int Set_Velocity_X(lua_State* p_state)
{
	cMovingSprite* p_sprite = *LuaWrap::check<cMovingSprite*>(p_state, 1);
	float new_velx = static_cast<float>(luaL_checknumber(p_state, 2));

	p_sprite->Set_Velocity(new_velx, p_sprite->m_vely); // Keep Y velocity
	return 0;
}

static int Get_Velocity_X(lua_State* p_state)
{
	cMovingSprite* p_sprite = *LuaWrap::check<cMovingSprite*>(p_state, 1);
	lua_pushnumber(p_state, p_sprite->m_velx);
	return 1;
}

static int Set_Velocity_Y(lua_State* p_state)
{
	cMovingSprite* p_sprite = *LuaWrap::check<cMovingSprite*>(p_state, 1);
	float new_vely = static_cast<float>(luaL_checknumber(p_state, 2));

	p_sprite->Set_Velocity(p_sprite->m_velx, new_vely); // Keep X velocity
	return 0;
}

static int Get_Velocity_Y(lua_State* p_state)
{
	cMovingSprite* p_sprite = *LuaWrap::check<cMovingSprite*>(p_state, 1);
	lua_pushnumber(p_state, p_sprite->m_vely);
	return 1;
}

static int Set_Velocity(lua_State* p_state)
{
	cMovingSprite* p_sprite = *LuaWrap::check<cMovingSprite*>(p_state, 1);
	float new_velx = static_cast<float>(luaL_checknumber(p_state, 2));
	float new_vely = static_cast<float>(luaL_checknumber(p_state, 3));

	p_sprite->Set_Velocity(new_velx, new_vely);
	return 0;
}

static int Get_Velocity(lua_State* p_state)
{
	cMovingSprite* p_sprite = *LuaWrap::check<cMovingSprite*>(p_state, 1);
	lua_pushnumber(p_state, p_sprite->m_velx);
	lua_pushnumber(p_state, p_sprite->m_vely);
	return 2;
}

static int Accelerate_X(lua_State* p_state)
{
	cMovingSprite* p_sprite = *LuaWrap::check<cMovingSprite*>(p_state, 1);
	float velx = static_cast<float>(luaL_checknumber(p_state, 2));

	p_sprite->Add_Velocity_X(velx);
	return 0;
}

static int Accelerate_Y(lua_State* p_state)
{
	cMovingSprite* p_sprite = *LuaWrap::check<cMovingSprite*>(p_state, 1);
	float vely = static_cast<float>(luaL_checknumber(p_state, 2));

	p_sprite->Add_Velocity_Y(vely);
	return 0;
}

static int Accelerate(lua_State* p_state)
{
	cMovingSprite* p_sprite = *LuaWrap::check<cMovingSprite*>(p_state, 1);
	float velx = static_cast<float>(luaL_checknumber(p_state, 2));
	float vely = static_cast<float>(luaL_checknumber(p_state, 3));

	p_sprite->Add_Velocity(velx, vely);
	return 0;
}

/***************************************
 * Binding
 ***************************************/

static luaL_Reg Methods[] = {
	{"accelerate",		Accelerate},
	{"accelerate_x",	Accelerate_X},
	{"accelerate_y",	Accelerate_Y},
	{"get_direction",	Get_Direction},
	{"get_velocity",	Get_Velocity},
	{"get_velocity_x",	Get_Velocity_X},
	{"get_velocity_y",	Get_Velocity_Y},
	{"set_direction",	Set_Direction},
	{"set_velocity",	Set_Velocity},
	{"set_velocity_x",	Set_Velocity_X},
	{"set_velocity_y",	Set_Velocity_Y},
	{NULL, NULL}
};

void Script::Open_Moving_Sprite(lua_State* p_state)
{
	LuaWrap::register_subclass<cMovingSprite>(	p_state,
												"MovingSprite",
												"Sprite",
												Methods,
												NULL,
												NULL,	// Not intended to be instanciated directly
												NULL);	// Memory managed by SMC
}
