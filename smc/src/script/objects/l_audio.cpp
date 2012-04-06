#include "../luawrap.hpp"
#include "../../audio/audio.h"
#include "l_audio.h"

using namespace SMC;

/***************************************
 * Methods
 ***************************************/

/**
 * play_sound( filename [, volume = -1 [, loops = 0 [, resid = -1 ] ] ] ) → a_bool
 *
 * Plays a sound.
 *
 * Arguments
 * ---------
 *
 * * filename: Path to the sound file to play, relative to the *pixmaps*
 *   directiory.
 * * volume (-1): Volume the sound shall have. Between 0 and 100.
 * * loops (0): Number of times to redo the sound, _after_ it has
 *   been played once, i.e. the effective number of sounds equals
 *   `1 + loops`.
 * * resid (-1): Special identifier to prevent a sound from being
 *   played while another instance of this sound is already being
 *   played. SMC ensures that no two sounds with the same resource
 *   ID are played at the same time, i.e. the running sound will
 *   be stopped and discarded before your sound is played. You can
 *   define your own IDs, but there is a number of IDs predefined
 *   by SMC:
 *
 *   1. Maryo jump sound.
 *   2. Maryo wall hit sound
 *   3. Maryo powerdown sound
 *   4. Maryo ball sound
 *   5. Maryo death sound
 *   6. Fireplant, blue mushroom, ghost mushroom mushroom and feather
 *      sound
 *   7. 1-Up mushroom and moon sound
 *   8. Maryo Au! (not used currently)
 *   9. Maryo stop sound
 *
 *   Specifying -1 for this parameter allows the given sound to be
 *   played multiple times.
 *
 * Return value
 * ------------
 * True on success, false otherwise. Possible failure reasons include
 * incorrect filenames or the sound may simply have been muted by
 * the user in SMC’s preferences, so you probably shouldn’t give
 * too much on this.
 */
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

/**
 * play_music( filename [, loops = 0 [, force = true [, fadein_ms = 0 ] ] ] ) → a_bool
 *
 * Plays the given music.
 *
 * Arguments
 * --------
 * * filename: Name of the suondfile to play, relative to the `music/`
 *   directory.
 * * loops (0): Number of times to repeat the music _after_ is has
 *   been played once, i.e. the number of musics played equals
 *   `1 + loops`.
 * * force (true): Enforces the new music to run even if another music
 *   is already running (the running music is stopped and discarded).
 *   Note this behaviour is enabled by default, you have to explicitely
 *   set this to `false`.
 * * fadein_ms (0): Number of milliseconds to fade the music in.
 *
 * Return value
 * ------------
 * True on success, false otherwise. Possible failure reasons include
 * incorrect filenames or the music may simply have been muted by
 * the user in SMC’s preferences, so you probably shouldn’t give
 * too much on this.
 */
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
