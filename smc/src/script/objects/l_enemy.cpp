#include "../luawrap.hpp"
#include "../../enemies/enemy.h"
#include "../../level/level.h"
#include "../../core/sprite_manager.h"
#include "../../gui/hud.h"
#include "../../audio/audio.h"
#include "l_sprite.h"
#include "l_enemy.h"

using namespace SMC;

/***************************************
 * Event handlers
 ***************************************/

LUA_IMPLEMENT_EVENT(die);

/***************************************
 * "Normal" access
 ***************************************/

/**
 * get_kill_points() → a_number
 *
 * Returns the number of points the player gains
 * after killing this enemy.
 */
static int Lua_Enemy_Get_Kill_Points(lua_State* p_state)
{
	cEnemy* p_enemy = *LuaWrap::check<cEnemy*>(p_state, 1);
	lua_pushnumber(p_state, p_enemy->m_kill_points);
	return 1;
}

/**
 * set_kill_points( points )
 *
 * Sets the number of points the player gains after killing
 * this enemy.
 */
static int Lua_Enemy_Set_Kill_Points(lua_State* p_state)
{
	cEnemy* p_enemy = *LuaWrap::check<cEnemy*>(p_state, 1);
	p_enemy->m_kill_points = static_cast<unsigned int>(luaL_checknumber(p_state, 2));
	return 0;
}

/**
 * get_kill_sound() → a_string
 *
 * Returns the filename of the sound to play when the enemy
 * gets killed.
 */
static int Lua_Enemy_Get_Kill_Sound(lua_State* p_state)
{
	cEnemy* p_enemy = *LuaWrap::check<cEnemy*>(p_state, 1);
	lua_pushstring(p_state, p_enemy->m_kill_sound.c_str());
	return 1;
}

/**
 * set_kill_sound( path )
 *
 * Sets the filename of the sound to play when the enemy
 * gets killed. `path' is relative to the sounds/ directory.
 */
static int Lua_Enemy_Set_Kill_Sound(lua_State* p_state)
{
	cEnemy* p_enemy = *LuaWrap::check<cEnemy*>(p_state, 1);
	std::string ksound = luaL_checkstring(p_state, 2);
	p_enemy->m_kill_sound = ksound;
	return 0;
}

/**
 * enable_fire_resistance()
 *
 * Makes this enemy resistant to fire.
 */
static int Lua_Enemy_Enable_Fire_Resistance(lua_State* p_state)
{
	cEnemy* p_enemy = *LuaWrap::check<cEnemy*>(p_state, 1);
	p_enemy->m_fire_resistant = true;
	return 0;
}

/**
 * disable_fire_resistance()
 *
 * Makes this enemy vulnerable to fire.
 */
static int Lua_Enemy_Disable_Fire_Resistance(lua_State* p_state)
{
	cEnemy* p_enemy = *LuaWrap::check<cEnemy*>(p_state, 1);
	p_enemy->m_fire_resistant = false;
	return 0;
}

/**
 * is_fire_resistant() → a_bool
 *
 * Checks whether this enemy is resistant to fire, and if so,
 * returns true. Otherwise, returns false.
 */
static int Lua_Enemy_Is_Fire_Resistant(lua_State* p_state)
{
	cEnemy* p_enemy = *LuaWrap::check<cEnemy*>(p_state, 1);
	if (p_enemy->m_fire_resistant)
		lua_pushboolean(p_state, 1);
	else
		lua_pushboolean(p_state, 0);

	return 1;
}

/**
 * kill()
 *
 * Immediately kills this enemy. No points are given to
 * the player (i.e. it is as if the enemy fell into an abyss
 * or something like that).
 *
 * Causes a subsequent Die event.
 *
 * See also: kill_with_points().
 */
static int Lua_Enemy_Kill(lua_State* p_state)
{
	cEnemy* p_enemy = *LuaWrap::check<cEnemy*>(p_state, 1);
	p_enemy->Set_Dead(true);
	return 0;
}

/**
 * kill_with_points()
 *
 * Immediately kills this enemy. Points are given to the
 * player. Also plays the dying sound for this enemy.
 *
 * Causes a subsequent Die event.
 *
 * See also: kill().
 */
static int Lua_Enemy_Kill_With_Points(lua_State* p_state)
{
	cEnemy* p_enemy = *LuaWrap::check<cEnemy*>(p_state, 1);
	pHud_Points->Add_Points(p_enemy->m_kill_points,
	                        p_enemy->m_pos_x,
	                        p_enemy->m_pos_y - 5.0f);
	pAudio->Play_Sound(p_enemy->m_kill_sound);
	p_enemy->Set_Dead(true);

	return 0;
}

/***************************************
 * Binding
 ***************************************/

static luaL_Reg Enemy_Methods[] = {
	{"disable_fire_resistance", Lua_Enemy_Disable_Fire_Resistance},
	{"enable_fire_resistance",  Lua_Enemy_Enable_Fire_Resistance},
	{"get_kill_points",         Lua_Enemy_Get_Kill_Points},
	{"get_kill_sound",          Lua_Enemy_Get_Kill_Sound},
	{"is_fire_resistant",       Lua_Enemy_Is_Fire_Resistant},
	{"kill",                    Lua_Enemy_Kill},
	{"kill_with_points",        Lua_Enemy_Kill_With_Points},
	{"on_die",                  LUA_EVENT_HANDLER(die)},
	{"set_kill_points",         Lua_Enemy_Set_Kill_Points},
	{"set_kill_sound",          Lua_Enemy_Set_Kill_Sound},
	{NULL, NULL}
};

void Script::Open_Enemy(lua_State* p_state)
{
	LuaWrap::register_subclass<cEnemy>(p_state,
	                                   "Enemy",
	                                   "AnimatedSprite",
	                                   Enemy_Methods,
	                                   NULL,
	                                   NULL,  // Not meant to be instanciated directly (but what about custom enemies?)
	                                   NULL); // Memory managed by SMC

	// Register the "__index" metamethod for Enemy
	lua_getglobal(p_state, "Enemy");
	lua_newtable(p_state);
	lua_pushstring(p_state, "__index");
	lua_pushcfunction(p_state, Sprite___Index<cEnemy>);
	lua_settable(p_state, -3);
	lua_setmetatable(p_state, -2);
	lua_pop(p_state, 1); // Remove the Sprite class table for balancing
}
