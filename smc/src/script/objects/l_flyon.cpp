// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../luawrap.hpp"
#include "../../level/level.h"
#include "../../core/sprite_manager.h"
#include "../../enemies/flyon.h"
#include "l_sprite.h"
#include "l_enemy.h"
#include "l_flyon.h"

using namespace SMC;

/***************************************
 * Class methods
 ***************************************/

static int Allocate(lua_State* p_state)
{
	// Check required arguments
	if (!lua_istable(p_state, 1))
		return luaL_error(p_state, "Argument #0 is not a class table.");
	std::string sdir = luaL_checkstring(p_state, 2);

	// Get required arguments
	ObjectDirection dir = DIR_UP;
	if (sdir == "up")
		dir = DIR_UP;
	else if (sdir == "right")
		dir = DIR_RIGHT;
	else if (sdir == "left")
		dir = DIR_LEFT;
	else if (sdir == "down")
		dir = DIR_DOWN;
	else
		return luaL_error(p_state, "Argument #1 '%s' is not a valid flyon direction.", sdir.c_str());

	// Create the userdata
	lua_pushvalue(p_state, 1); // Needed for set_imethod_table()
	cFlyon** pp_flyon	= (cFlyon**) lua_newuserdata(p_state, sizeof(cFlyon*));
	cFlyon*  p_flyon	= new cFlyon(pActive_Level->m_sprite_manager);
	*pp_flyon			= p_flyon;

	LuaWrap::InternalC::set_imethod_table(p_state); // Attach instance methods
	// Remove the duplicated class table
	lua_insert(p_state, -2);
	lua_pop(p_state, 1);

	// This is a generated object
	p_flyon->Set_Spawned(true);

	// Required arguments
	p_flyon->Set_Direction(dir);

	// Optional arguments
	if (lua_isstring(p_state, 3))
		p_flyon->Set_Image_Dir(lua_tostring(p_state, 3));
	if (lua_isnumber(p_state, 4)){
		int uid = static_cast<int>(lua_tonumber(p_state, 4));
		if (pActive_Level->m_sprite_manager->Is_UID_In_Use(uid))
			return luaL_error(p_state, "UID %d is already in use.", uid);
		p_flyon->m_uid = uid;
	}

	// Let SMC manage the memory
	pActive_Level->m_sprite_manager->Add(p_flyon);

	return 1;
}

/***************************************
 * Instance methods
 ***************************************/

static int Get_Max_Distance(lua_State* p_state)
{
	cFlyon* p_flyon = *LuaWrap::check<cFlyon*>(p_state, 1);
	lua_pushnumber(p_state, p_flyon->m_max_distance);
	return 1;
}

static int Set_Max_Distance(lua_State* p_state)
{
	cFlyon* p_flyon	= *LuaWrap::check<cFlyon*>(p_state, 1);
	float mdist		= static_cast<float>(luaL_checknumber(p_state, 2));

	p_flyon->Set_Max_Distance(mdist);

	return 0;
}

static int Get_Image_Dir(lua_State* p_state)
{
	cFlyon* p_flyon = *LuaWrap::check<cFlyon*>(p_state, 1);
	lua_pushstring(p_state, p_flyon->m_img_dir.c_str());
	return 1;
}

static int Set_Image_Dir(lua_State* p_state)
{
	cFlyon* p_flyon = *LuaWrap::check<cFlyon*>(p_state, 1);
	std::string dir = luaL_checkstring(p_state, 2);
	p_flyon->Set_Image_Dir(dir);
	return 0;
}

static int Get_Speed(lua_State* p_state)
{
	cFlyon* p_flyon = *LuaWrap::check<cFlyon*>(p_state, 1);
	lua_pushnumber(p_state, p_flyon->m_speed);
	return 1;
}

static int Set_Speed(lua_State* p_state)
{
	cFlyon* p_flyon	= *LuaWrap::check<cFlyon*>(p_state, 1);
	float speed		= static_cast<float>(luaL_checknumber(p_state, 2));

	p_flyon->Set_Speed(speed);

	return 0;
}

static int Is_Moving_Back(lua_State* p_state)
{
	cFlyon* p_flyon = *LuaWrap::check<cFlyon*>(p_state, 1);
	lua_pushboolean(p_state, p_flyon->m_move_back);
	return 1;
}

static int Wait_Time(lua_State* p_state)
{
	cFlyon* p_flyon = *LuaWrap::check<cFlyon*>(p_state, 1);
	lua_pushnumber(p_state, p_flyon->m_wait_time);
	return 1;
}

/***************************************
 * Binding
 ***************************************/

static luaL_Reg Methods[] = {
	{"get_image_dir", Get_Image_Dir},
	{"get_max_distance", Get_Max_Distance},
	{"get_speed", Get_Speed},
	{"is_moving_back", Is_Moving_Back},
	{"set_image_dir", Set_Image_Dir},
	{"set_max_distance", Set_Max_Distance},
	{"set_speed", Set_Speed},
	{"wait_time", Wait_Time},
	{NULL, NULL}
};

void Script::Open_Flyon(lua_State* p_state)
{
	LuaWrap::register_subclass<cFlyon>(	p_state,
										"Flyon",
										"Enemy",
										Methods,
										NULL,
										Allocate,
										NULL); // Memory managed by SMC

	// Register the "__index" metamethod for Flyon
	lua_getglobal(p_state, "Flyon");
	lua_newtable(p_state);
	lua_pushstring(p_state, "__index");
	lua_pushcfunction(p_state, Sprite___Index<cFlyon>);
	lua_settable(p_state, -3);
	lua_setmetatable(p_state, -2);
	lua_pop(p_state, 1); // Remove the table for balancing
}
