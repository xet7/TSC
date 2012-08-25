// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../script.h"
#include "../../level/level.h"
#include "../../core/sprite_manager.h"
#include "../../level/level_player.h"
#include "../events/event.h"
#include "l_sprite.h"

using namespace SMC;

/***************************************
 * Class methods
 ***************************************/

static int Allocate(lua_State* p_state)
{
	if (!lua_istable(p_state, 1))
		return luaL_error(p_state, "No class table given.");

	/* Create a new sprite and store a pointer to the allocated sprite
	 * in the Lua object, because the sprite is garbage-collected automatically
	 * by the SMC core when freeing the SpriteManager the sprite belongs
	 * to, so it mustn’t be freed by Lua previously what would result
	 * in a segmentation fault. */
	cSprite** pp_sprite	= (cSprite**) lua_newuserdata(p_state, sizeof(cSprite*));
	*pp_sprite			= new cSprite(pActive_Level->m_sprite_manager);
	cSprite* p_sprite	= *pp_sprite;

	LuaWrap::InternalC::set_imethod_table(p_state, 1); // Attach instance methods

	// Handle optional image argument
	if (lua_isstring(p_state, 2))
		p_sprite->Set_Image(pVideo->Get_Surface(lua_tostring(p_state, 2)), true);
	// Handle optional UID argument
	if (lua_isnumber(p_state, 3)){
		int uid = static_cast<int>(lua_tonumber(p_state, 5));
		if (pActive_Level->m_sprite_manager->Is_UID_In_Use(uid))
			return luaL_error(p_state, "UID %d is already used.", uid);

		p_sprite->m_uid = uid;
	}

	// Default massivity type is front passive
	p_sprite->Set_Sprite_Type(TYPE_FRONT_PASSIVE);

	// Hidden by default
	p_sprite->Set_Active(false);

	// This is a generated object that should neither be saved
	// nor should it be editable in the editor.
	p_sprite->Set_Spawned(true);

	// Add to the sprite manager for automatic memory management by SMC
	pActive_Level->m_sprite_manager->Add(p_sprite);

	return 1;
}

/***************************************
 * Event handlers
 ***************************************/

// Event definitions
LUA_IMPLEMENT_EVENT(touch);

/***************************************
 * "Normal" access
 ***************************************/

static int Show(lua_State* p_state)
{
	cSprite* p_sprite = *LuaWrap::check<cSprite*>(p_state, 1);
	p_sprite->Set_Active(true);
	return 0;
}

static int Hide(lua_State* p_state)
{
	cSprite* p_sprite = *LuaWrap::check<cSprite*>(p_state, 1);
	p_sprite->Set_Active(false);
	return 0;
}

static int Get_UID(lua_State* p_state)
{
	cSprite* p_sprite = *LuaWrap::check<cSprite*>(p_state, 1);
	lua_pushnumber(p_state, p_sprite->m_uid);
	return 1;
}

static int Set_Massive_Type(lua_State* p_state)
{
	cSprite* p_sprite	= *LuaWrap::check<cSprite*>(p_state, 1);
	std::string type	= luaL_checkstring(p_state, 2);

	if (type == "passive")
		p_sprite->Set_Sprite_Type(TYPE_PASSIVE);
	else if (type == "frontpassive" || type == "front_passive") // Official: "front_passive"
		p_sprite->Set_Sprite_Type(TYPE_FRONT_PASSIVE);
	else if (type == "massive")
		p_sprite->Set_Sprite_Type(TYPE_MASSIVE);
	else if (type == "halfmassive" || type == "half_massive") // Official: "halfmassive"
		p_sprite->Set_Sprite_Type(TYPE_HALFMASSIVE);
	else if (type == "climbable")
		p_sprite->Set_Sprite_Type(TYPE_CLIMBABLE);
	else // Non-standard types like TYPE_ENEMY are not allowed here
		return luaL_error(p_state, "Invalid type '%s'.", type.c_str());

	return 0;
}

static int Get_X(lua_State* p_state)
{
	lua_pushnumber(p_state, (*LuaWrap::check<cSprite*>(p_state, 1))->m_pos_x);
	return 1;
}

static int Get_Start_X(lua_State* p_state)
{
	lua_pushnumber(p_state, (*LuaWrap::check<cSprite*>(p_state, 1))->m_start_pos_x);
	return 1;
}

static int Set_X(lua_State* p_state)
{
	cSprite* p_sprite = *LuaWrap::check<cSprite*>(p_state, 1);
	float new_x = static_cast<float>(luaL_checknumber(p_state, 2));
	p_sprite->Set_Pos_X(new_x);
	return 0;
}

static int Set_Start_X(lua_State* p_state)
{
	cSprite* p_sprite = *LuaWrap::check<cSprite*>(p_state, 1);
	float new_x = static_cast<float>(luaL_checknumber(p_state, 2));
	p_sprite->Set_Pos_X(new_x, true);
	return 0;
}

static int Get_Y(lua_State* p_state)
{
	lua_pushnumber(p_state, (*LuaWrap::check<cSprite*>(p_state, 1))->m_pos_y);
	return 1;
}

static int Get_Start_Y(lua_State* p_state)
{
	lua_pushnumber(p_state, (*LuaWrap::check<cSprite*>(p_state, 1))->m_start_pos_y);
	return 1;
}

static int Set_Y(lua_State* p_state)
{
	cSprite* p_sprite = *LuaWrap::check<cSprite*>(p_state, 1);
	float new_y = static_cast<float>(luaL_checknumber(p_state, 2));
	p_sprite->Set_Pos_Y(new_y);
	return 0;
}

static int Set_Start_Y(lua_State* p_state)
{
	cSprite* p_sprite = *LuaWrap::check<cSprite*>(p_state, 1);
	float new_y = static_cast<float>(luaL_checknumber(p_state, 2));
	p_sprite->Set_Pos_Y(new_y, true);
	return 0;
}

static int Get_Z(lua_State* p_state)
{
	lua_pushnumber(p_state, (*LuaWrap::check<cSprite*>(p_state, 1))->m_pos_z);
	return 1;
}

static int Pos(lua_State* p_state)
{
	lua_pushnumber(p_state, (*LuaWrap::check<cSprite*>(p_state, 1))->m_pos_x);
	lua_pushnumber(p_state, (*LuaWrap::check<cSprite*>(p_state, 1))->m_pos_y);
	return 2;
}

static int Start_Pos(lua_State* p_state)
{
	lua_pushnumber(p_state, (*LuaWrap::check<cSprite*>(p_state, 1))->m_start_pos_x);
	lua_pushnumber(p_state, (*LuaWrap::check<cSprite*>(p_state, 1))->m_start_pos_y);
	return 2;
}

static int Get_Rect(lua_State* p_state)
{
	cSprite* p_sprite = *LuaWrap::check<cSprite*>(p_state, 1);

	lua_pushnumber(p_state, p_sprite->m_rect.m_x);
	lua_pushnumber(p_state, p_sprite->m_rect.m_y);
	lua_pushnumber(p_state, p_sprite->m_rect.m_w);
	lua_pushnumber(p_state, p_sprite->m_rect.m_h);

	return 4;
}

static int Get_Collision_Rect(lua_State* p_state)
{
	cSprite* p_sprite = *LuaWrap::check<cSprite*>(p_state, 1);

	lua_pushnumber(p_state, p_sprite->m_col_rect.m_x);
	lua_pushnumber(p_state, p_sprite->m_col_rect.m_y);
	lua_pushnumber(p_state, p_sprite->m_col_rect.m_w);
	lua_pushnumber(p_state, p_sprite->m_col_rect.m_h);

	return 4;
}

static int Warp(lua_State* p_state)
{
	cSprite* p_sprite = *LuaWrap::check<cSprite*>(p_state, 1);

	float new_x = static_cast<float>(luaL_checknumber(p_state, 2));
	float new_y = static_cast<float>(luaL_checknumber(p_state, 3));

	p_sprite->Set_Pos(new_x, new_y);

	return 0;
}

static int Start_At(lua_State* p_state)
{
	cSprite* p_sprite = *LuaWrap::check<cSprite*>(p_state, 1);

	float new_x = static_cast<float>(luaL_checknumber(p_state, 2));
	float new_y = static_cast<float>(luaL_checknumber(p_state, 3));

	p_sprite->Set_Pos(new_x, new_y, true);

	return 0;
}

static int Is_Player(lua_State* p_state)
{
	cSprite* p_sprite = *LuaWrap::check<cSprite*>(p_state, 1);

	if (p_sprite == pLevel_Player)
		lua_pushboolean(p_state, 1);
	else
		lua_pushboolean(p_state, 0);

	return 1;
}

/***************************************
 * Binding
 ***************************************/

static luaL_Reg Methods[] = {
	{"get_collision_rect",	Get_Collision_Rect},
	{"get_rect",			Get_Rect},
	{"get_start_x",			Get_Start_X},
	{"get_start_y",			Get_Start_Y},
	{"get_uid",				Get_UID},
	{"get_x",				Get_X},
	{"get_y",				Get_Y},
	{"get_z",				Get_Z},
	{"hide",				Hide},
	{"is_player",			Is_Player},
	{"on_touch",			LUA_EVENT_HANDLER(touch)},
	{"pos",					Pos},
	{"set_massive_type",	Set_Massive_Type},
	{"set_start_x",			Set_Start_X},
	{"set_start_y",			Set_Start_Y},
	{"set_x",				Set_X},
	{"set_y",				Set_Y},
	{"show",				Show},
	{"start_at",			Start_At},
	{"start_pos",			Start_Pos},
	{"warp",				Warp},
	{NULL, NULL}
};

void Script::Open_Sprite(lua_State* p_state)
{
	LuaWrap::register_subclass<cSprite>(	p_state,
											"Sprite",
											"Object",
											Methods,
											NULL,
											Allocate,
											NULL); // Memory managed by SMC (Sprite) and Lua (pointer to Sprite)
}

