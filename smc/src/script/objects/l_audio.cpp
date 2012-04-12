#include "../luawrap.hpp"
#include "../../audio/audio.h"
#include "l_audio.h"

using namespace SMC;

/***************************************
 * Methods
 ***************************************/

static int Play_Sound(lua_State* p_state)
{
	std::string filename = luaL_checkstring(p_state, 2);
	int volume					 = -1;
	int loops						 = 0;
	int resid						 = -1;

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
	std::string filename = luaL_checkstring(p_state, 2);
	int loops						 = 0;
	bool force					 = true;
	int fadein_ms				 = 0;

	if (lua_isnumber(p_state, 3))
		loops = static_cast<int>(lua_tonumber(p_state, 3));
	if (lua_isboolean(p_state, 4))
		force = lua_toboolean(p_state, 4);
	if (lua_isnumber(p_state, 5))
		fadein_ms = static_cast<unsigned int>(lua_tonumber(p_state, 5));

	bool res = pAudio->Play_Music(filename, loops, force, fadein_ms);
	lua_pushboolean(p_state, res);
	
	return 1;
}

/***************************************
 * Binding
 ***************************************/

luaL_Reg Methods[] = {
	{"play_music", Play_Music},
	{"play_sound", Play_Sound},
	{NULL, NULL}
};

void Script::Open_Audio(lua_State* p_state)
{
	LuaWrap::register_singleton(p_state, "Audio", Methods);
}
