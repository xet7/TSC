// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../script.h"
#include "../../level/level.h"
#include "../../core/sprite_manager.h"
#include "../../enemies/enemy.h"
#include "../../gui/hud.h"
#include "../events/event.h"
#include "l_enemy.h"

using namespace SMC;

/***************************************
 * Event handlers
 ***************************************/

LUA_IMPLEMENT_EVENT(die);

/***************************************
 * "Normal" access
 ***************************************/

static int Get_Kill_Points(lua_State* p_state)
{
	cEnemy* p_enemy = *LuaWrap::check<cEnemy*>(p_state, 1);
	lua_pushnumber(p_state, p_enemy->m_kill_points);
	return 1;
}

static int Set_Kill_Points(lua_State* p_state)
{
	cEnemy* p_enemy = *LuaWrap::check<cEnemy*>(p_state, 1);
	p_enemy->m_kill_points = static_cast<unsigned int>(luaL_checknumber(p_state, 2));
	return 0;
}

static int Get_Kill_Sound(lua_State* p_state)
{
	cEnemy* p_enemy = *LuaWrap::check<cEnemy*>(p_state, 1);
	lua_pushstring(p_state, p_enemy->m_kill_sound.c_str());
	return 1;
}

static int Set_Kill_Sound(lua_State* p_state)
{
	cEnemy* p_enemy = *LuaWrap::check<cEnemy*>(p_state, 1);
	std::string ksound = luaL_checkstring(p_state, 2);
	p_enemy->m_kill_sound = ksound;
	return 0;
}

static int Enable_Fire_Resistance(lua_State* p_state)
{
	cEnemy* p_enemy = *LuaWrap::check<cEnemy*>(p_state, 1);
	p_enemy->m_fire_resistant = true;
	return 0;
}

static int Disable_Fire_Resistance(lua_State* p_state)
{
	cEnemy* p_enemy = *LuaWrap::check<cEnemy*>(p_state, 1);
	p_enemy->m_fire_resistant = false;
	return 0;
}

static int Is_Fire_Resistant(lua_State* p_state)
{
	cEnemy* p_enemy = *LuaWrap::check<cEnemy*>(p_state, 1);
	if (p_enemy->m_fire_resistant)
		lua_pushboolean(p_state, 1);
	else
		lua_pushboolean(p_state, 0);

	return 1;
}

static int Kill(lua_State* p_state)
{
	cEnemy* p_enemy = *LuaWrap::check<cEnemy*>(p_state, 1);
	p_enemy->Set_Dead(true);
	return 0;
}

static int Kill_With_Points(lua_State* p_state)
{
	cEnemy* p_enemy = *LuaWrap::check<cEnemy*>(p_state, 1);
	pHud_Points->Add_Points(	p_enemy->m_kill_points,
								p_enemy->m_pos_x,
								p_enemy->m_pos_y - 5.0f);
	pAudio->Play_Sound(p_enemy->m_kill_sound);
	p_enemy->Set_Dead(true);

	return 0;
}

/***************************************
 * Binding
 ***************************************/

static luaL_Reg Methods[] = {
	{"disable_fire_resistance",	Disable_Fire_Resistance},
	{"enable_fire_resistance",	Enable_Fire_Resistance},
	{"get_kill_points",			Get_Kill_Points},
	{"get_kill_sound",			Get_Kill_Sound},
	{"is_fire_resistant",		Is_Fire_Resistant},
	{"kill",					Kill},
	{"kill_with_points",		Kill_With_Points},
	{"on_die",					LUA_EVENT_HANDLER(die)},
	{"set_kill_points",			Set_Kill_Points},
	{"set_kill_sound",			Set_Kill_Sound},
	{NULL, NULL}
};

void Script::Open_Enemy(lua_State* p_state)
{
	LuaWrap::register_subclass<cEnemy>(	p_state,
										"Enemy",
										"AnimatedSprite",
										Methods,
										NULL,
										NULL,	// Not meant to be instanciated directly (but what about custom enemies?)
										NULL);	// Memory managed by SMC
}
