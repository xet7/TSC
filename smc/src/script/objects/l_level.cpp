// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../script.h"
#include "../../level/level.h"
#include "../../user/savegame.h"
#include "../../core/property_helper.h"
#include "../events/event.h"
#include "l_level.h"

using namespace SMC;

/***************************************
 * Events
 ***************************************/

LUA_IMPLEMENT_EVENT(save)
LUA_IMPLEMENT_EVENT(load)

/***************************************
 * Methods
 ***************************************/

static int Get_Author(lua_State* p_state)
{
	lua_pushstring(p_state, pActive_Level->m_author.c_str());
	return 1;
}

static int Get_Description(lua_State* p_state)
{
	lua_pushstring(p_state, pActive_Level->m_description.c_str());
	return 1;
}

static int Get_Difficulty(lua_State* p_state)
{
	lua_pushnumber(p_state, pActive_Level->m_difficulty);
	return 1;
}

static int Get_Engine_Version(lua_State* p_state)
{
	lua_pushnumber(p_state, pActive_Level->m_engine_version);
	return 1;
}

static int Get_Filename(lua_State* p_state)
{
	lua_pushstring(p_state, path_to_utf8(pActive_Level->m_level_filename).c_str());
	return 1;
}

static int Get_Music_Filename(lua_State* p_state)
{
	std::string	format;
	std::string	result;
	bool		with_ext;

	if (!lua_isstring(p_state, 2))
		format = "remove_nothing";
	else
		format = lua_tostring(p_state, 2);
	with_ext = lua_toboolean(p_state, 3);

	if (format == "remove_complete_dir")
		result = pActive_Level->Get_Music_Filename(0, with_ext);
	else if (format == "remove_music_dir")
		result = pActive_Level->Get_Music_Filename(1, with_ext);
	else if (format == "remove_nothing")
		result = pActive_Level->Get_Music_Filename(2, with_ext);
	else
		return luaL_error(p_state, "Invalid directory format specifier '%s'.", format.c_str());

	lua_pushstring(p_state, result.c_str());
	return 1;
}

static int Get_Next_Level_Filename(lua_State* p_state)
{
	lua_pushstring(p_state, path_to_utf8(pActive_Level->m_next_level_filename).c_str());
	return 1;
}

static int Get_Script(lua_State* p_state)
{
	lua_pushstring(p_state, pActive_Level->m_script.c_str());
	return 1;
}

static int Finish(lua_State* p_state)
{
	pLevel_Manager->Finish_Level(lua_toboolean(p_state, 2));
	return 0;
}

/***************************************
 * Binding
 ***************************************/

static luaL_Reg Methods[] = {
	{"get_author",				Get_Author},
	{"get_description",			Get_Description},
	{"get_difficulty",			Get_Difficulty},
	{"get_engine_version",		Get_Engine_Version},
	{"get_filename",			Get_Filename},
	{"get_music_filename",		Get_Music_Filename},
	{"get_next_level_filename", Get_Next_Level_Filename},
	{"get_script",				Get_Script},
	{"finish",					Finish},
	{"on_load",					LUA_EVENT_HANDLER(load)},
	{"on_save",					LUA_EVENT_HANDLER(save)},
	{NULL, NULL}
};

void Script::Open_Level(lua_State* p_state)
{
	LuaWrap::register_subclass<cLevel>(	p_state,
										"LevelClass",
										"Object",
										Methods,
										NULL,
										NULL, // Singleton, can’t be instanciated
										NULL); // Memory managed by SMC

	// Make the global variable Level point to the
	// sole instance of the LevelClass class.
	lua_getglobal(p_state, "LevelClass"); // Class table needed
	// The Lua `Level' variable actually points to pSavegame instead
	// of pActiveLevel, because that one holds the event table
	// for the level events, which needs to be easily accessible
	// when saving/loading a game.
	cSavegame** pp_savegame = (cSavegame**) lua_newuserdata(p_state, sizeof(cSavegame*));
	*pp_savegame = pSavegame;
	LuaWrap::InternalC::set_imethod_table(p_state, -2); // Attach instance method table
	// Cleanup the stack, remove the class table
	lua_insert(p_state, -2);
	lua_pop(p_state, 1);
	// Make the global variable
	lua_setglobal(p_state, "Level");
}
