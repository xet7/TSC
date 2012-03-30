#include "../luawrap.hpp"
#include "player.h"
#include "../../level/level_player.h"

using namespace SMC;

/**
 * downgrade()
 *
 * Hurt the player. Kills him if he is small.
 */
static int Lua_Player_Downgrade(lua_State* p_state)
{
	if (!lua_istable(p_state, 1))
		return luaL_error(p_state, "No singleton table given.");

	pLevel_Player->DownGrade();
	return 0;
}

/**
 * set_type( type )
 *
 * Apply a powerup to the player (or shrink him). The possible
 * types you can pass are the following strings:
 *
 * "dead":  Please use the kill() method instead.
 * "small": Please use the downgrade() method instead.
 * "big":   Apply the normal mushroom.
 * "fire":  Apply the fireplant.
 * "ice"    Apply the ice mushroom.
 * "ghost": Apply the ghost mushroom.
 */
static int Lua_Player_Set_Type(lua_State* p_state)
{
	if (!lua_istable(p_state, 1))
		return luaL_error(p_state, "No singleton table given.");

	std::string type_str = std::string(luaL_checkstring(p_state, 2));
	Maryo_type type;

	if (type_str == "dead")
		type = MARYO_DEAD;
	else if (type_str == "small")
		type = MARYO_SMALL;
	else if (type_str == "big")
		type = MARYO_BIG;
	else if (type_str == "fire")
		type = MARYO_FIRE;
	else if (type_str == "ice")
		type = MARYO_ICE;
	//else if (type_str == "cape") // Not implemented officially by SMC
	//	type = MARY_CAPE;
	else if (type_str == "ghost")
		type = MARYO_GHOST;
	else
		return luaL_error(p_state, "Invalid type '%s'.", type_str.c_str());

	pLevel_Player->Set_Type(type);

	return 0;
}

/**
 * Kill()
 *
 * Immediately kill the player.
 */
static int Lua_Player_Kill(lua_State* p_state)
{
	if (!lua_istable(p_state, 1))
		return luaL_error(p_state, "No singleton table given.");

	pLevel_Player->DownGrade(true);
	return 0;
}

/**
 * Warp(new_x, new_y)
 *
 * Warp the player somewhere. Note you are responsible for ensuring the
 * coordinates are valid, this method behaves exactly as a level entry
 * (i.e. doesn’t check coordinate validness).
 *
 * You can easily get the coordinates by moving around the cursor in
 * the SMC level editor.
 */
static int Lua_Player_Warp(lua_State* p_state)
{
	if (!lua_istable(p_state, 1))
		return luaL_error(p_state, "No singleton table given.");

	float new_x = static_cast<float>(luaL_checklong(p_state, 2));
	float new_y = static_cast<float>(luaL_checklong(p_state, 3));

	pLevel_Player->Set_Pos(new_x, new_y);
	pLevel_Player->Clear_Collisions();

	return 0;
}

static luaL_Reg Player_Methods[] = {
	{"downgrade", Lua_Player_Downgrade},
	{"kill", Lua_Player_Kill},
	{"set_type", Lua_Player_Set_Type},
	{"warp", Lua_Player_Warp},
	{NULL, NULL}
};

void Script::Open_Player(lua_State* p_state)
{
	LuaWrap::register_singleton(p_state, "Player", Player_Methods);
}
