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
 * Creates a new particle emitter.
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
 * Set the Z coordinate.
 */
static int Set_Z(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	float z = static_cast<float>(luaL_checknumber(p_state, 2));

	p_emitter->Set_Pos_Z(z);

	return 0;
}

/**
 * set_image( path )
 *
 * Set the path of the image to emit. Relative to the pixmaps/
 * directory.
 */
static int Set_Image(lua_State* p_state)
{
	cParticle_Emitter* p_emitter = *LuaWrap::check<cParticle_Emitter*>(p_state, 1);
	std::string path = luaL_checkstring(p_state, 2);

	p_emitter->Set_Image(pVideo->Get_Surface(path));

	return 0;
}

/**
 * set_time_to_live( time [, rand ] )
 *
 * Set the time this emitter may live, plusminus `rand'. Both
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
 * emit()
 *
 * Do it! Do it! Emit particles!
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
	{"emit",						 Emit},
	{"set_image",				 Set_Image},
	{"set_time_to_live", Set_Time_To_Live},
	{"set_z",						 Set_Z},
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
