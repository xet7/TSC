// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../script.h"
#include "../../level/level.h"
#include "../../core/sprite_manager.h"
#include "../../enemies/gee.h"
#include "l_sprite.h"
#include "l_enemy.h"
#include "l_gee.h"

using namespace SMC;

/***************************************
 * Class methods
 ***************************************/

static int Allocate(lua_State* p_state)
{
	if (!lua_istable(p_state, 1))
		return luaL_error(p_state, "No class table given.");
	std::string sdir = luaL_checkstring(p_state, 2);

	// Get required arguments
	ObjectDirection dir;
	if (sdir == "horizontal")
		dir = DIR_HORIZONTAL;
	else if (sdir == "vertical")
		dir = DIR_VERTICAL;
	else
		return luaL_error(p_state, "'%s' is not a valid gee direction.", sdir.c_str());

	// Create the userdata
	cGee** pp_gee	= (cGee**) lua_newuserdata(p_state, sizeof(cGee*));
	cGee*  p_gee	= new cGee(pActive_Level->m_sprite_manager);
	*pp_gee			= p_gee;

	LuaWrap::InternalC::set_imethod_table(p_state, 1); // Attach instance methods

	// This is a generated object
	p_gee->Set_Spawned(true);

	// Required arguments
	p_gee->Set_Direction(dir);

	// Optional arguments
	if (lua_isstring(p_state, 3)){
		std::string color = lua_tostring(p_state, 3);
		if (color == "yellow")
			p_gee->Set_Color(COL_YELLOW);
		else if (color == "red")
			p_gee->Set_Color(COL_RED);
		else if (color == "green")
			p_gee->Set_Color(COL_GREEN);
		else
			return luaL_error(p_state, "Invalid gee color %s.", color.c_str());
	}

	// Let SMC manage the memory
	pActive_Level->m_sprite_manager->Add(p_gee);

	return 1;
}

/***************************************
 * Instance methods
 ***************************************/

static int Get_Max_Distance(lua_State* p_state)
{
	cGee* p_gee = *LuaWrap::check<cGee*>(p_state, 1);
	lua_pushnumber(p_state, p_gee->m_max_distance);
	return 1;
}

static int Set_Max_Distance(lua_State* p_state)
{
	cGee* p_gee = *LuaWrap::check<cGee*>(p_state, 1);
	int max_distance = luaL_checkint(p_state, 2);
	p_gee->Set_Max_Distance(max_distance);
	return 0;
}

static int Get_Color(lua_State* p_state)
{
	cGee* p_gee = *LuaWrap::check<cGee*>(p_state, 1);

	std::string color;
	switch(p_gee->m_color_type){
	case COL_YELLOW:
		color = "yellow";
		break;
	case COL_RED:
		color = "red";
		break;
	case COL_GREEN:
		color = "green";
		break;
	default:
		return luaL_error(p_state, "Invalid gee color detected.");
		break;
	}

	lua_pushstring(p_state, color.c_str());

	return 1;
}

static int Set_Color(lua_State* p_state)
{
	cGee* p_gee = *LuaWrap::check<cGee*>(p_state, 1);
	std::string color = luaL_checkstring(p_state, 2);

	if (color == "yellow")
		p_gee->Set_Color(COL_YELLOW);
	else if (color == "red")
		p_gee->Set_Color(COL_RED);
	else if (color == "green")
		p_gee->Set_Color(COL_GREEN);
	else
		return luaL_error(p_state, "Invalid gee color %s.", color.c_str());

	return 0;
}

static int Get_Fly_Speed(lua_State* p_state)
{
	cGee* p_gee = *LuaWrap::check<cGee*>(p_state, 1);
	lua_pushnumber(p_state, p_gee->m_speed_fly);
	return 1;
}

static int Set_Fly_Speed(lua_State* p_state)
{
	cGee* p_gee = *LuaWrap::check<cGee*>(p_state, 1);
	float fly_speed = static_cast<float>(luaL_checknumber(p_state, 2));
	p_gee->m_speed_fly = fly_speed;
	return 0;
}

static int Is_Always_Flying(lua_State* p_state)
{
	cGee* p_gee = *LuaWrap::check<cGee*>(p_state, 1);
	lua_pushboolean(p_state, p_gee->m_always_fly);
	return 1;
}

static int Enable_Always_Flying(lua_State* p_state)
{
	cGee* p_gee = *LuaWrap::check<cGee*>(p_state, 1);
	p_gee->m_always_fly = true;
	return 0;
}

static int Disable_Always_Flying(lua_State* p_state)
{
	cGee* p_gee = *LuaWrap::check<cGee*>(p_state, 1);
	p_gee->m_always_fly = false;
	return 0;
}

static int Get_Wait_Time(lua_State* p_state)
{
	cGee* p_gee = *LuaWrap::check<cGee*>(p_state, 1);
	lua_pushnumber(p_state, p_gee->m_wait_time);
	return 1;
}

static int Set_Wait_Time(lua_State* p_state)
{
	cGee* p_gee = *LuaWrap::check<cGee*>(p_state, 1);
	float wait_time = static_cast<float>(luaL_checknumber(p_state, 2));
	p_gee->m_wait_time = wait_time;
	return 0;
}

static int Get_Fly_Distance(lua_State* p_state)
{
	cGee* p_gee = *LuaWrap::check<cGee*>(p_state, 1);
	lua_pushnumber(p_state, p_gee->m_fly_distance);
	return 1;
}

static int Set_Fly_Distance(lua_State* p_state)
{
	cGee* p_gee = *LuaWrap::check<cGee*>(p_state, 1);
	int distance = luaL_checkint(p_state, 2);
	p_gee->m_fly_distance = distance;
	return 0;
}

static int Activate(lua_State* p_state)
{
	cGee* p_gee = *LuaWrap::check<cGee*>(p_state, 1);
	p_gee->Activate();
	return 0;
}

static int Stop(lua_State* p_state)
{
	cGee* p_gee = *LuaWrap::check<cGee*>(p_state, 1);
	p_gee->Stop();
	return 0;
}

static int Is_At_Max_Distance(lua_State* p_state)
{
	cGee* p_gee = *LuaWrap::check<cGee*>(p_state, 1);
	lua_pushboolean(p_state, p_gee->Is_At_Max_Distance());
	return 1;
}

/***************************************
 * Binding
 ***************************************/

static luaL_Reg Methods[] = {
	{"activate", Activate},
	{"disable_always_flying", Disable_Always_Flying},
	{"enable_always_flying", Enable_Always_Flying},
	{"get_color", Get_Color},
	{"get_fly_distance", Get_Fly_Distance},
	{"get_fly_speed", Get_Fly_Speed},
	{"get_max_distance", Get_Max_Distance},
	{"get_wait_time", Get_Wait_Time},
	{"is_always_flying", Is_Always_Flying},
	{"is_at_max_distance", Is_At_Max_Distance},
	{"set_color", Set_Color},
	{"set_fly_distance", Set_Fly_Distance},
	{"set_fly_speed", Set_Fly_Speed},
	{"set_max_distance", Set_Max_Distance},
	{"set_wait_time", Set_Wait_Time},
	{"stop", Stop},
	{NULL, NULL}
};

void Script::Open_Gee(lua_State* p_state)
{
	LuaWrap::register_subclass<cGee>(	p_state,
										"Gee",
										"Enemy",
										Methods,
										NULL,
										Allocate,
										NULL); // Memory managed by SMC
}
