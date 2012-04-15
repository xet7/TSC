// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../luawrap.hpp"
#include "../../enemies/eato.h"
#include "../../level/level.h"
#include "../../core/sprite_manager.h"
#include "l_sprite.h"
#include "l_enemy.h"
#include "l_eato.h"

using namespace SMC;

/***************************************
 * Class methods
 ***************************************/

static int Allocate(lua_State* p_state)
{
	// Get required arguments
	if (!lua_istable(p_state, 1))
		return luaL_error(p_state, "Argument #0 is not a class table.");
	std::string sdir = luaL_checkstring(p_state, 2);

	// Check required arguments
	ObjectDirection dir;
	if (sdir == "up_left")
		dir = DIR_UP_LEFT;
	else if (sdir == "up_right")
		dir = DIR_UP_RIGHT;
	else if (sdir == "left_up")
		dir = DIR_LEFT_UP;
	else if (sdir == "left_down")
		dir = DIR_LEFT_DOWN;
	else if (sdir == "right_up")
		dir = DIR_RIGHT_UP;
	else if (sdir == "right_down")
		dir = DIR_RIGHT_DOWN;
	else if (sdir == "down_left")
		dir = DIR_DOWN_LEFT;
	else if (sdir == "down_right")
		dir = DIR_DOWN_RIGHT;
	else
		return luaL_error(p_state, "Argument #1 '%s' is not a valid eato direction.", sdir.c_str());

	// Optional arguments
	std::string directory = "enemy/eato/brown/";
	if (lua_isstring(p_state, 3))
		directory = lua_tostring(p_state, 3);

	// Create the userdata
	lua_pushvalue(p_state, 1); // Needed for set_imethod_table()
	cEato** pp_eato	= (cEato**) lua_newuserdata(p_state, sizeof(cEato*));
	cEato*	p_eato	= new cEato(pActive_Level->m_sprite_manager);
	*pp_eato		= p_eato;

	LuaWrap::InternalC::set_imethod_table(p_state); // Attach instance methods
	// Remove the duplicated class table
	lua_insert(p_state, -2);
	lua_pop(p_state, 1);

	// This is a generated object
	p_eato->Set_Spawned(true);

	// Initialization
	p_eato->Set_Direction(dir);
	p_eato->Set_Image_Dir(directory);

	// Let SMC manage the memory
	pActive_Level->m_sprite_manager->Add(p_eato);

	return 1;
}

/***************************************
 * Instance methods
 ***************************************/

static int Get_Image_Dir(lua_State* p_state)
{
	cEato* p_eato = *LuaWrap::check<cEato*>(p_state, 1);
	lua_pushstring(p_state, p_eato->m_img_dir.c_str());
	return 1;
}

static int Set_Image_Dir(lua_State* p_state)
{
	cEato* p_eato = *LuaWrap::check<cEato*>(p_state, 1);
	std::string dir = luaL_checkstring(p_state, 2);
	p_eato->Set_Image_Dir(dir);
	return 0;
}

/***************************************
 * Binding
 ***************************************/

static luaL_Reg Methods[] = {
	{"get_image_dir",	Get_Image_Dir},
	{"set_image_dir",	Set_Image_Dir},
	{NULL, NULL}
};

void Script::Open_Eato(lua_State* p_state)
{
	LuaWrap::register_subclass<cEato>	(p_state,
										"Eato",
										"Enemy",
										Methods,
										NULL,
										Allocate,
										NULL); // Memory managed by SMC

	// Register the "__index" metamethod for Eato
	lua_getglobal(p_state, "Eato");
	lua_newtable(p_state);
	lua_pushstring(p_state, "__index");
	lua_pushcfunction(p_state, Sprite___Index<cEato>);
	lua_settable(p_state, -3);
	lua_setmetatable(p_state, -2);
	lua_pop(p_state, 1); // Remove the class table for balancing
}
