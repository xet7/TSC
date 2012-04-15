// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../luawrap.hpp"
#include "../../objects/sprite.h"
#include "../../level/level.h"
#include "../../level/level_player.h"
#include "../../core/sprite_manager.h"
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
	lua_pushvalue(p_state, 1); // Needed for attaching the instance methods
	cSprite** pp_sprite	= (cSprite**) lua_newuserdata(p_state, sizeof(cSprite*));
	*pp_sprite			= new cSprite(pActive_Level->m_sprite_manager);
	cSprite* p_sprite	= *pp_sprite;

	// Attach instance methods
	LuaWrap::InternalC::set_imethod_table(p_state);

	// Remove the duplicated class table
	lua_insert(p_state, -2);
	lua_pop(p_state, 1);

	// Handle optional image argument
	if (lua_isstring(p_state, 2))
		p_sprite->Set_Image(pVideo->Get_Surface(lua_tostring(p_state, 2)), true);
	// Handle optional X coordinate argument
	if (lua_isnumber(p_state, 3))
		p_sprite->Set_Pos_X(luaL_checkint(p_state, 3), true);
	// Handle optional Y coordinate argument
	if (lua_isnumber(p_state, 4))
		p_sprite->Set_Pos_Y(luaL_checkint(p_state, 4), true);
	// Handle optional UID argument
	if (lua_isnumber(p_state, 5)){
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
	cSprite* p_sprite = *LuaWrap::check<cSprite*>(p_state, 1); // Note dereferencing
	const char* str = luaL_checkstring(p_state, 2);
	if (!lua_isfunction(p_state, 3))
		return luaL_error(p_state, "No function given.");

	lua_pushvalue(p_state, 3); // Don’t remove the argument (keep the stack balanced)
	int ref = luaL_ref(p_state, LUA_REGISTRYINDEX);

	// Add the event handler to the list (if the request event key
  // doesn’t yet exist, it will automatically be created).
	p_sprite->m_event_table[str].push_back(ref);

	return 0;
}

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

static int Set_X(lua_State* p_state)
{
	cSprite* p_sprite = *LuaWrap::check<cSprite*>(p_state, 1);
	float new_x = static_cast<float>(luaL_checknumber(p_state, 2));
	p_sprite->Set_Pos_X(new_x);
	return 0;
}

static int Get_Y(lua_State* p_state)
{
	lua_pushnumber(p_state, (*LuaWrap::check<cSprite*>(p_state, 1))->m_pos_y);
	return 1;
}

static int Set_Y(lua_State* p_state)
{
	cSprite* p_sprite = *LuaWrap::check<cSprite*>(p_state, 1);
	float new_y = static_cast<float>(luaL_checknumber(p_state, 2));
	p_sprite->Set_Pos_Y(new_y);
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
	{"get_uid",				Get_UID},
	{"get_x",				Get_X},
	{"get_y",				Get_Y},
	{"get_z",				Get_Z},
	{"hide",				Hide},
	{"is_player",			Is_Player},
	{"on_touch",			LUA_EVENT_HANDLER(touch)},
	{"pos",					Pos},
	{"register",			Register},
	{"set_massive_type",	Set_Massive_Type},
	{"set_x",				Set_X},
	{"set_y",				Set_Y},
	{"show",				Show},
	{"warp",				Warp},
	{NULL, NULL}
};

void Script::Open_Sprite(lua_State* p_state)
{
	LuaWrap::register_class<cSprite>(	p_state,
										"Sprite",
										Methods,
										NULL,
										Allocate,
										NULL); // Memory managed by SMC (Sprite) and Lua (pointer to Sprite)

	// Register the "__index" metamethod for Sprite
	lua_getglobal(p_state, "Sprite");
	lua_newtable(p_state);
	lua_pushstring(p_state, "__index");
	lua_pushcfunction(p_state, Sprite___Index<cSprite>);
	lua_settable(p_state, -3);
	lua_setmetatable(p_state, -2);
	lua_pop(p_state, 1); // Remove the Sprite class table for balancing
}

/***************************************
 * Helper methods
 ***************************************/

/**
 * Helper method that just forwards a Lua call to register() with
 * the given name. Expects the given stack to look like this:
 *	 [1] Receiver (self, a userdata)
 *	 [2] The Lua function handler
 *
 * The IMPLEMENT_LUA_EVENT macro just defines a function that
 * calls this function with the argument given to it converted
 * to a string.
 */
int Script::Forward_To_Sprite_Register(lua_State* p_state, std::string event_name)
{
	if (!lua_isuserdata(p_state, 1)) // self
		return luaL_error(p_state, "No receiver (userdata) given.");
	if (!lua_isfunction(p_state, 2)) // handler function
		return luaL_error(p_state, "No function given.");

	// Get register() function
	lua_pushstring(p_state, "register");
	lua_gettable(p_state, 1);
	// Forward to register()
	lua_pushvalue(p_state, 1); // self
	lua_pushstring(p_state, event_name.c_str());
	lua_pushvalue(p_state, 2); // function
	lua_call(p_state, 3, 0);

	return 0;
}
