// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../script.h"
#include "../../audio/audio.h"
#include "l_audio.h"

using namespace SMC;

/***************************************
 * Methods
 ***************************************/

static int Play_Sound(lua_State* p_state)
{
	std::string filename	= luaL_checkstring(p_state, 2);
	int volume				= -1;
	int loops				= 0;
	int resid				= -1;

	if (lua_isnumber(p_state, 3))
		volume = static_cast<int>(lua_tonumber(p_state, 3));
	if (lua_isnumber(p_state, 4))
		loops = static_cast<int>(lua_tonumber(p_state, 4));
	if (lua_isnumber(p_state, 5))
		resid = static_cast<int>(lua_tonumber(p_state, 5));

	bool res = pAudio->Play_Sound(filename, resid, volume, loops);
	lua_pushboolean(p_state, res);

	return 1;
}

static int Play_Music(lua_State* p_state)
{
	std::string filename	= luaL_checkstring(p_state, 2);
	int loops				= 0;
	bool force				= true;
	int fadein_ms			= 0;

	if (lua_isnumber(p_state, 3))
		loops = static_cast<int>(lua_tonumber(p_state, 3));
	if (lua_isboolean(p_state, 4))
		force = static_cast<bool>(lua_toboolean(p_state, 4));
	if (lua_isnumber(p_state, 5))
		fadein_ms = static_cast<unsigned int>(lua_tonumber(p_state, 5));

	bool res = pAudio->Play_Music(filename, loops, force, fadein_ms);
	lua_pushboolean(p_state, res);
	
	return 1;
}

/***************************************
 * Binding
 ***************************************/

static luaL_Reg Methods[] = {
	{"play_music",	Play_Music},
	{"play_sound",	Play_Sound},
	{NULL, NULL}
};

void Script::Open_Audio(lua_State* p_state)
{
	LuaWrap::register_class<cAudio>(	p_state,
										"AudioClass",
										Methods,
										NULL,
										NULL,	// Singleton, can’t be instanciated
										NULL);	// Memory managed by SMC

	// Make the global variable Audio point to the sole instance
	// of the AudioClass class.
	lua_getglobal(p_state, "AudioClass"); // Class table needed for the instance methods
	cAudio** pp_audio = (cAudio**) lua_newuserdata(p_state, sizeof(cAudio*));
	*pp_audio = pAudio;
	LuaWrap::InternalC::set_imethod_table(p_state, -2); // Attach instance methods
	// Cleanup the stack, remove the class table
	lua_insert(p_state, -2);
	lua_pop(p_state, 1);
	// Make the global variable
	lua_setglobal(p_state, "Audio");
}
