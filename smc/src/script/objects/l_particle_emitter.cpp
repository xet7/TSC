// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../script.h"
#include "../../level/level.h"
#include "../../core/sprite_manager.h"
#include "../../core/property_helper.h"
#include "l_particle_emitter.h"

using namespace SMC;

/***************************************
 * Class methods
 ***************************************/

static int Allocate(lua_State* p_state)
{
	// Check required arguments
	if (!lua_istable(p_state, 1))
		return luaL_error(p_state, "No class table given.");
	float x = static_cast<float>(luaL_checknumber(p_state, 2));
	float y = static_cast<float>(luaL_checknumber(p_state, 3));

	// Check optional arguments
	float width = 0.0f;
	float height = 0.0f;
	if (lua_isnumber(p_state, 4))
		width = static_cast<float>(lua_tonumber(p_state, 4));
	if (lua_isnumber(p_state, 5))
		height = static_cast<float>(lua_tonumber(p_state, 5));

	// Create the userdata
	cParticle_Emitter** pp_emitter	= (cParticle_Emitter**) lua_newuserdata(p_state, sizeof(cParticle_Emitter*));
	cParticle_Emitter* p_emitter	= new cParticle_Emitter(pActive_Level->m_sprite_manager);
	*pp_emitter						= p_emitter;

	LuaWrap::InternalC::set_imethod_table(p_state, 1); // Attach instance methods

	// Initialize the emitter’s default values
	p_emitter->Set_Emitter_Rect(x, y, width, height);

	// This is a generated object
	p_emitter->Set_Spawned(true);

	// Let SMC manage the memory
	pActive_Animation_Manager->Add(p_emitter);

	return 1;
}

/***************************************
 * Instance methods
 ***************************************/

static int Set_Z(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	float z = static_cast<float>(luaL_checknumber(p_state, 2));

	p_emitter->Set_Pos_Z(z);

	return 0;
}

static int Get_Image_Filename(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	lua_pushstring(p_state, p_emitter->m_image_filename.c_str());
	return 1;
}

static int Set_Image_Filename(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	std::string path = luaL_checkstring(p_state, 2);

	p_emitter->Set_Image_Filename(utf8_to_path(path));

	return 0;
}

static int Get_Time_To_Live(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	lua_pushnumber(p_state, p_emitter->m_time_to_live_rand);
	lua_pushnumber(p_state, p_emitter->m_time_to_live);
	return 2;
}

static int Set_Time_To_Live(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	float time = static_cast<float>(luaL_checknumber(p_state, 2));
	float rand = 0.0f;
	if (lua_isnumber(p_state, 3))
		rand = static_cast<float>(lua_tonumber(p_state, 3));

	p_emitter->Set_Time_to_Live(time, rand);

	return 0;
}

static int Get_Scale(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	lua_pushnumber(p_state, p_emitter->m_size_scale_rand);
	lua_pushnumber(p_state, p_emitter->m_size_scale);
	return 2;
}

static int Set_Scale(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	float scale = static_cast<float>(luaL_checknumber(p_state, 2));
	float rand = 0.0f;
	if (lua_isnumber(p_state, 3))
		rand = static_cast<float>(lua_tonumber(p_state, 3));

	p_emitter->Set_Scale(scale, rand);

	return 0;
}

static int Get_Speed(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	lua_pushnumber(p_state, p_emitter->m_vel_rand);
	lua_pushnumber(p_state, p_emitter->m_vel);
	return 2;
}

static int Set_Speed(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	float speed = static_cast<float>(luaL_checknumber(p_state, 2));
	float rand = 2.0f; // Default value used by cParticle_Emitter itself
	if (lua_isnumber(p_state, 3))
		rand = static_cast<float>(lua_tonumber(p_state, 3));

	p_emitter->Set_Speed(speed, rand);

	return 0;
}

static int Get_Emitter_Time_To_Live(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	lua_pushnumber(p_state, p_emitter->m_emitter_time_to_live);
	return 1;
}

static int Set_Emitter_Time_To_Live(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	float time = static_cast<float>(luaL_checknumber(p_state, 2));

	p_emitter->Set_Emitter_Time_to_Live(time);

	return 0;
}

static int Get_Quota(lua_State* p_state)
{
  cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
  lua_pushnumber(p_state, p_emitter->m_emitter_quota);
  return 1;
}

static int Set_Quota(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	unsigned int quota = static_cast<unsigned int>(luaL_checknumber(p_state, 2));

	p_emitter->Set_Quota(quota);

	return 0;
}

static int Set_Gravity_X(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	float xgravity	= static_cast<float>(luaL_checknumber(p_state, 2));
	float xrand		= 0.0f;

	if (lua_isnumber(p_state, 3))
		xrand = static_cast<float>(lua_tonumber(p_state, 3));

	p_emitter->Set_Horizontal_Gravity(xgravity, xrand);

	return 0;
}

static int Get_Gravity_X(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	lua_pushnumber(p_state, p_emitter->m_gravity_x);
	lua_pushnumber(p_state, p_emitter->m_gravity_x_rand);
	return 2;
}

static int Set_Gravity_Y(lua_State* p_state)
{
	cParticle_Emitter* p_emitter	= *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	float ygravity					= static_cast<float>(luaL_checknumber(p_state, 2));
	float yrand						= 0.0f;

	if (lua_isnumber(p_state, 3))
		yrand = static_cast<float>(luaL_checknumber(p_state, 3));

	p_emitter->Set_Vertical_Gravity(ygravity, yrand);

	return 0;
}

static int Get_Gravity_Y(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	lua_pushnumber(p_state, p_emitter->m_gravity_y);
	lua_pushnumber(p_state, p_emitter->m_gravity_y_rand);
	return 2;
}

static int Get_Const_Rotation_X(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);

	lua_pushnumber(p_state, p_emitter->m_const_rot_x);
	lua_pushnumber(p_state, p_emitter->m_const_rot_x_rand);

	return 2;
}

static int Set_Const_Rotation_X(lua_State* p_state)
{
	cParticle_Emitter* p_emitter	= *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	float rot						= static_cast<float>(luaL_checknumber(p_state, 2));
	float rand						= 0.0f;

	if (lua_isnumber(p_state, 3))
		rand = static_cast<float>(lua_tonumber(p_state, 3));

	p_emitter->Set_Const_Rotation_X(rot, rand);

	return 0;
}

static int Get_Const_Rotation_Y(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);

	lua_pushnumber(p_state, p_emitter->m_const_rot_y);
	lua_pushnumber(p_state, p_emitter->m_const_rot_y_rand);

	return 2;
}

static int Set_Const_Rotation_Y(lua_State* p_state)
{
	cParticle_Emitter* p_emitter	= *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	float rot						= static_cast<float>(luaL_checknumber(p_state, 2));
	float rand						= 0.0f;

	if (lua_isnumber(p_state, 3))
		rand = static_cast<float>(lua_tonumber(p_state, 3));

	p_emitter->Set_Const_Rotation_Y(rot, rand);

	return 0;
}

static int Get_Const_Rotation_Z(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);

	lua_pushnumber(p_state, p_emitter->m_const_rot_z);
	lua_pushnumber(p_state, p_emitter->m_const_rot_z_rand);

	return 2;
}

static int Set_Const_Rotation_Z(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	float rot	 = static_cast<float>(luaL_checknumber(p_state, 2));
	float rand = 0.0f;

	if (lua_isnumber(p_state, 3))
		rand = static_cast<float>(lua_tonumber(p_state, 3));

	p_emitter->Set_Const_Rotation_Z(rot, rand);

	return 0;
}

static int Emit(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	p_emitter->Emit();
	return 0;
}

/***************************************
 * Binding
 ***************************************/

static luaL_Reg Methods[] = {
	{"emit",						Emit},
	{"get_const_rotation_x",		Get_Const_Rotation_X},
	{"get_const_rotation_y",		Get_Const_Rotation_Y},
	{"get_const_rotation_z",		Get_Const_Rotation_Z},
	{"get_emitter_time_to_live",	Get_Emitter_Time_To_Live},
	{"get_gravity_x",				Get_Gravity_X},
	{"get_gravity_y",				Get_Gravity_Y},
	{"get_image_filename",			Get_Image_Filename},
	{"get_quota",					Get_Quota},
	{"get_scale",					Get_Scale},
	{"get_speed",					Get_Speed},
	{"get_time_to_live",			Get_Time_To_Live},
	{"set_const_rotation_x",		Set_Const_Rotation_X},
	{"set_const_rotation_y",		Set_Const_Rotation_Y},
	{"set_const_rotation_z",		Set_Const_Rotation_Z},
	{"set_emitter_time_to_live",	Set_Emitter_Time_To_Live},
	{"set_gravity_x",				Set_Gravity_X},
	{"set_gravity_y",				Set_Gravity_Y},
	{"set_image_filename",			Set_Image_Filename},
	{"set_quota",					Set_Quota},
	{"set_scale",					Set_Scale},
	{"set_speed",					Set_Speed},
	{"set_time_to_live",			Set_Time_To_Live},
	{"set_z",						Set_Z},
	{NULL, NULL}
};

void Script::Open_Particle_Emitter(lua_State* p_state)
{
	LuaWrap::register_class<cParticle_Emitter>(	p_state,
												"ParticleEmitter",
												Methods,
												NULL,
												Allocate,
												NULL);
}
