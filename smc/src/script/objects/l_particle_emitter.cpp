#include "../luawrap.hpp"
#include "../../level/level.h"
#include "../../video/animation.h"
#include "../../video/video.h"
#include "l_particle_emitter.h"

using namespace SMC;

/***************************************
 * Class methods
 ***************************************/

/**
 * new( x, y [, width [, height ] ] ) → a_particle_emitter
 *
 * Creates a new particle emitter. It won’t emit particles by default,
 * you first have to adjust the emitter with the various set_* methods,
 * and when you’ve done this you can either call emit() which will
 * gives you absolute control over each emitted particle, or use
 * set_emitter_time_to_live() to make the emitter emit particles
 * automatically.
 */
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
	lua_pushvalue(p_state, 1); // Need the class table for attaching instance methods
	cParticle_Emitter** pp_emitter = (cParticle_Emitter**) lua_newuserdata(p_state, sizeof(cParticle_Emitter*));
	cParticle_Emitter* p_emitter	 = new cParticle_Emitter(pActive_Level->m_sprite_manager);
	*pp_emitter										 = p_emitter;

	LuaWrap::InternalC::set_imethod_table(p_state); // Attach instance methods

	// Remove the duplicated class table
	lua_insert(p_state, -2);
	lua_pop(p_state, 1);

	// Initialize the emitter’s default values
	p_emitter->Set_Emitter_Rect(x, y, width, height);

	// Let SMC manage the memory
	pActive_Animation_Manager->Add(p_emitter);

	return 1;
}

/***************************************
 * Instance methods
 ***************************************/

/**
 * set_z( val )
 *
 * Set the Z coordinate. Note get_z() is implemented directly
 * in Sprite.
 */
static int Set_Z(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	float z = static_cast<float>(luaL_checknumber(p_state, 2));

	p_emitter->Set_Pos_Z(z);

	return 0;
}

/**
 * get_image_filename() → a_string
 *
 * Returns the path to the currently emitted particle’s
 * image file, relative to the pixmaps/ directory.
 */
static int Get_Image_Filename(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	lua_pushstring(p_state, p_emitter->m_image_filename.c_str());
	return 1;
}

/**
 * set_image_filename( path )
 *
 * Set the path of the image to emit. Relative to the pixmaps/
 * directory.
 */
static int Set_Image_Filename(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	std::string path = luaL_checkstring(p_state, 2);

	p_emitter->Set_Image_Filename(path);

	return 0;
}

/**
 * get_time_to_live() → a_number, another_number
 *
 * Return the TTL of this particle emitter’s particles. The second value
 * returned is the random time addition (see set_time_to_live()).
 */
static int Get_Time_To_Live(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	lua_pushnumber(p_state, p_emitter->m_time_to_live_rand);
	lua_pushnumber(p_state, p_emitter->m_time_to_live);
	return 2;
}

/**
 * set_time_to_live( time [, rand ] )
 *
 * Set the time the particles may live, plusminus `rand'. Both
 * values are seconds.
 */
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

/**
 * get_scale() → a_number, another_number
 *
 * Returns the scaling information. `a_number' is the main
 * scalation (0.5 = half, 1 = original, 2 = double, anything
 * in between accordingly), `another_number' is the random
 * scalation variation.
 */
static int Get_Scale(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	lua_pushnumber(p_state, p_emitter->m_size_scale_rand);
	lua_pushnumber(p_state, p_emitter->m_size_scale);
	return 2;
}

/**
 * set_scale( scale [, rand ] )
 *
 * Set the scale of the emitted particles. 1 means original size,
 * 0.5 is half the original size and 2 is the double size.
 * `rand' may be used to vary the emitted particles’ sizes.
 * `scale' has to be between 0 and 100.
 */
static int Set_Scale(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	float scale = static_cast<float>(luaL_checknumber(p_state, 2));
	float rand = 0.0f;
	if (lua_isnumber(p_state, 3));
	rand = static_cast<float>(lua_tonumber(p_state, 3));

	p_emitter->Set_Scale(scale, rand);

	return 0;
}

/**
 * get_speed() → a_number, another_number
 *
 * Return the current emitting speed (`a_number') and the
 * random speed variation (`another_number').
 */
static int Get_Speed(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	lua_pushnumber(p_state, p_emitter->m_vel_rand);
	lua_pushnumber(p_state, p_emitter->m_vel);
	return 2;
}

/**
 * set_speed( speed [, rand ] )
 *
 * Set the emitting speed (in seconds), plusminus a random
 * variation.
 */
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

/**
 * get_emitter_time_to_live() → a_number
 *
 * Returns the number of seconds the whole emitter may live.
 * Don’t confuse with get_time_to_live(), which is for particles.
 * -1 means live forever.
 */
static int Get_Emitter_Time_To_Live(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	lua_pushnumber(p_state, p_emitter->m_emitter_time_to_live);
	return 1;
}

/**
 * set_emitter_time_to_live( time )
 *
 * Set the time the whole emitter may live (don’t confuse
 * this with set_time_to_live(), which is for the particles).
 * `time' is in seconds, and -1 means to make the emitter
 * stay forever.
 */
static int Set_Emitter_Time_To_Live(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	float time = static_cast<float>(luaL_checknumber(p_state, 2));

	p_emitter->Set_Emitter_Time_to_Live(time);

	return 0;
}

/**
 * get_quota() → a_number
 *
 * Returns the amount of particles emitted at one time.
 */
static int Get_Quota(lua_State* p_state)
{
  cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
  lua_pushnumber(p_state, p_emitter->m_emitter_quota);
  return 1;
}

/**
 * set_quota( quota )
 *
 * Sets the amount of particles emitted at one time.
 */
static int Set_Quota(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	unsigned int quota = static_cast<unsigned int>(luaL_checknumber(p_state, 2));

	p_emitter->Set_Quota(quota);

	return 0;
}

/**
 * set_gravity_x( gravity [, rand ] )
 *
 * Sets the horizontal gravity for this particle emitter. `rand' will be
 * added/subtracted to `gravity' on a random base.
 */
static int Set_Gravity_X(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	float xgravity = static_cast<float>(luaL_checknumber(p_state, 2));
	float xrand		 = 0.0f;

	if (lua_isnumber(p_state, 3))
		xrand = static_cast<float>(lua_tonumber(p_state, 3));

	p_emitter->Set_Horizontal_Gravity(xgravity, xrand);

	return 0;
}

/**
 * get_gravity_x() → gravity, rand
 *
 * Returns the base horizontal gravity and the random variation.
 */
static int Get_Gravity_X(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	lua_pushnumber(p_state, p_emitter->m_gravity_x);
	lua_pushnumber(p_state, p_emitter->m_gravity_x_rand);
	return 2;
}

/**
 * set_gravity_y( gravity [, rand ] )
 *
 * Sets the vertical gravity for this particle emitter. `rand' will be
 * added/subtracted to `gravity' on a random base.
 */
static int Set_Gravity_Y(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	float ygravity = static_cast<float>(luaL_checknumber(p_state, 2));
	float yrand		 = 0.0f;

	if (lua_isnumber(p_state, 3))
		yrand = static_cast<float>(luaL_checknumber(p_state, 3));

	p_emitter->Set_Vertical_Gravity(ygravity, yrand);

	return 0;
}

/**
 * get_gravity_y() → gravity, rand
 *
 * Returns the base vertical gravity and the random variation.
 */
static int Get_Gravity_Y(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	lua_pushnumber(p_state, p_emitter->m_gravity_y);
	lua_pushnumber(p_state, p_emitter->m_gravity_y_rand);
	return 2;
}

/**
 * emit()
 *
 * Emit a single particle. Usually you want to use
 * set_emitter_time_to_live() to make it emit particles automatically
 * for a certain period of time instead.
 */
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
	{"emit",						   Emit},
	{"get_emitter_time_to_live", Get_Emitter_Time_To_Live},
	{"get_gravity_x",      Get_Gravity_X},
	{"get_gravity_y",      Get_Gravity_Y},
	{"get_image_filename", Get_Image_Filename},
	{"get_quota",          Get_Quota},
	{"get_scale",          Get_Scale},
	{"get_speed",          Get_Speed},
	{"get_time_to_live",   Get_Time_To_Live},
	{"set_emitter_time_to_live", Set_Emitter_Time_To_Live},
	{"set_gravity_x",      Set_Gravity_X},
	{"set_gravity_y",      Set_Gravity_Y},
	{"set_image_filename", Set_Image_Filename},
	{"set_quota",          Set_Quota},
	{"set_scale",          Set_Scale},
	{"set_speed",          Set_Speed},
	{"set_time_to_live",   Set_Time_To_Live},
	{"set_z",						   Set_Z},
	{NULL, NULL}
};

void Script::Open_Particle_Emitter(lua_State* p_state)
{
	LuaWrap::register_class<cParticle_Emitter>(p_state,
	                                           "ParticleEmitter",
	                                           Methods,
	                                           NULL,
	                                           Allocate,
	                                           NULL);
}
