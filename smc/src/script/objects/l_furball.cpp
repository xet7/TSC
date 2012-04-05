#include "../luawrap.hpp"
#include "../../enemies/furball.h"
#include "../../level/level.h"
#include "../../core/sprite_manager.h"
#include "l_sprite.h"
#include "l_enemy.h"
#include "l_furball.h"

using namespace SMC;

/***************************************
 * Class methods
 ***************************************/

/**
 * new( direction [, color = "brown"] ) → a_furball
 *
 * Creates a new Furball. `direction' is either "left"
 * or "right", `color' may be one of:
 *
 * * "brown": Normal furball.
 * * "blue": More ice-resistant furball.
 * * "black": Furball boss.
 */
static int Allocate(lua_State* p_state)
{
	// Get required arguments
	if (!lua_istable(p_state, 1))
		return luaL_error(p_state, "Argument #0 is not a class table.");
	std::string sdir = luaL_checkstring(p_state, 2);

	// Check required arguments
	ObjectDirection dir;
	if (sdir == "right")
		dir = DIR_RIGHT;
	else if (sdir == "left")
		dir = DIR_LEFT;
	else
		return luaL_error(p_state, "Argument #1 '%s' is not a valid furball direction.", sdir.c_str());

	// Get & check optional arguments
	DefaultColor color = COL_BROWN;
	if (lua_isstring(p_state, 3)){
		std::string scolor = lua_tostring(p_state, 3);
		if (scolor == "brown")
			color = COL_BROWN;
		else if (scolor == "blue")
			color = COL_BLUE;
		else if (scolor == "black") // Boss
			color = COL_BLACK;
		else
			return luaL_error(p_state, "Argument #2 '%s' is not a valid furball color.", scolor.c_str());
	}

	// Create the userdata
	lua_pushvalue(p_state, 1); // Needed for set_imethod_table()
	cFurball** pp_furball = (cFurball**) lua_newuserdata(p_state, sizeof(cFurball*));
	cFurball*	 p_furball	= new cFurball(pActive_Level->m_sprite_manager);
	*pp_furball						= p_furball;

	LuaWrap::InternalC::set_imethod_table(p_state); // Attach instance methods

	// Remove the duplicated class table
	lua_insert(p_state, -2);
	lua_pop(p_state, 1);

	// This is a generated object
	p_furball->Set_Spawned(true);

	// Initialization
	p_furball->Set_Direction(dir);
	p_furball->Set_Color(color);

	// Let SMC manage the memory
	pActive_Level->m_sprite_manager->Add(p_furball);
	
	return 1;
}

/***************************************
 * Event handlers
 ***************************************/

LUA_IMPLEMENT_EVENT(downgrade);

/***************************************
 * "Normal" access
 ***************************************/

/**
 * get_color() → a_string.
 *
 * Returns the furball’s current appeareance. See set_color()
 * for a list of possible strings returned.
 */
static int Get_Color(lua_State* p_state)
{
	cFurball* p_furball = *LuaWrap::check<cFurball*>(p_state, 1);

	switch(p_furball->m_color_type){
	case COL_BROWN:
		lua_pushstring(p_state, "brown");
		break;
	case COL_BLUE:
		lua_pushstring(p_state, "blue");
		break;
	case COL_BLACK:
		lua_pushstring(p_state, "black");
		break;
	default:
		return luaL_error(p_state, "Unknown furball color detected!");
		break;
	}

	return 1;
}

/**
 * set_color( color )
 *
 * Change a furball’s appeareance. Note calling this also resets
 * any damage made to this enemy. `color' may be one of:
 *
 * * "brown": The normal furball.
 * * "blue":	The more ice-resistant furball.
 * * "black": The giant furball boss.
 */
static int Set_Color(lua_State* p_state)
{
	cFurball* p_furball = *LuaWrap::check<cFurball*>(p_state, 1);
	std::string color		= luaL_checkstring(p_state, 2);

	if (color == "brown")
		p_furball->Set_Color(COL_BROWN);
	else if (color == "blue")
		p_furball->Set_Color(COL_BLUE);
	else if (color == "black")
		p_furball->Set_Color(COL_BLACK);
	else
		return luaL_error(p_state, "Argument #1 '%s' is not a valid furball color.", color.c_str());

	return 0;
}

/**
 * enable_level_ends_if_killed()
 *
 * Activates automatic level ending when this furball
 * is killed. If you call this on a furball that isn’t
 * a boss (i.e. its color is "black"), raises an error.
 */
static int Enable_Level_Ends_If_Killed(lua_State* p_state)
{
	cFurball* p_furball = *LuaWrap::check<cFurball*>(p_state, 1);
	if (p_furball->m_type != TYPE_FURBALL_BOSS)
		return luaL_error(p_state, "This is not a furball boss.");

	p_furball->Set_Level_Ends_If_Killed(true);
	return 0;
}

/**
 * disable_level_ends_if_killed()
 *
 * Deactivates automatic level ending when this furball
 * is killed. If you call this on a furball that isn’t
 * a boss (i.e. its color is "black"), raises an error.
 */
static int Disable_Level_Ends_If_Killed(lua_State* p_state)
{
	cFurball* p_furball = *LuaWrap::check<cFurball*>(p_state, 1);
	if (p_furball->m_type != TYPE_FURBALL_BOSS)
		return luaL_error(p_state, "This is not a furball boss.");

	p_furball->Set_Level_Ends_If_Killed(false);
	return 0;
}

/**
 * does_level_end_if_killed() → a_bool
 *
 * For a normal, i.e. non-black, furball always returns false.
 * Otherwise, checks if defeating this boss furball causes
 * the level to end and if so, returns true, else returns
 * false.
 */
static int Does_Level_End_If_Killed(lua_State* p_state)
{
	cFurball* p_furball = *LuaWrap::check<cFurball*>(p_state, 1);

	// If it’s not a boss, the level doesn’t end anyway.
	if (p_furball->m_type != TYPE_FURBALL_BOSS)
		lua_pushboolean(p_state, false);
	else
		lua_pushboolean(p_state, p_furball->Level_Ends_If_Killed());
			
	return 1;
}

/**
 * set_max_downgrade_count( count )
 *
 * Sets the number of downgrades Maryo has to give this
 * furball boss before it dies. If this is not a furball
 * boss, raises an error.
 */
static int Set_Max_Downgrade_Count(lua_State* p_state)
{
	cFurball* p_furball = *LuaWrap::check<cFurball*>(p_state, 1);
	int count						= luaL_checkint(p_state, 2);

	if (p_furball->m_type != TYPE_FURBALL_BOSS)
		return luaL_error(p_state, "This is not a furball boss.");

	p_furball->Set_Max_Downgrade_Count(count);

	return 0;
}

/**
 * get_max_downgrade_count() → a_number
 *
 * Returns the maximum number of times this furball boss
 * needs to be hit before it dies (excluding higher forces
 * such as abysses). If this isn’t a furball boss, the
 * returned value is meaningless and most probably zero.
 */
static int Get_Max_Downgrade_Count(lua_State* p_state)
{
	cFurball* p_furball = *LuaWrap::check<cFurball*>(p_state, 1);

	lua_pushnumber(p_state, p_furball->Get_Max_Downgrade_Count());

	return 1;
}

/**
 * get_downgrade_count() → a_number
 *
 * Returns the number of times this furball boss has already
 * been downgraded. If this is not a furball boss, raises
 * an error.
 */
static int Get_Downgrade_Count(lua_State* p_state)
{
	cFurball* p_furball = *LuaWrap::check<cFurball*>(p_state, 1);

	if (p_furball->m_type != TYPE_FURBALL_BOSS)
		return luaL_error(p_state, "This is not a furball boss.");

	lua_pushnumber(p_state, p_furball->Get_Downgrade_Count());

	return 1;
}

/**
 * is_boss() → a_boolean
 *
 * Checks whether this furball is the giant, black and
 * incredibly horrible FURBALL BOSS!
 *
 * ...if so, hurries to return true, otherwise returns false.
 */
static int Is_Boss(lua_State* p_state)
{
	cFurball* p_furball = *LuaWrap::check<cFurball*>(p_state, 1);

	if (p_furball->m_type == TYPE_FURBALL_BOSS)
		lua_pushboolean(p_state, true);
	else
		lua_pushboolean(p_state, false);

	return 1;
}

/***************************************
 * Binding
 ***************************************/

static luaL_Reg Methods[] = {
	{"disable_level_ends_if_killed", Disable_Level_Ends_If_Killed},
	{"does_level_end_if_killed",     Does_Level_End_If_Killed},
	{"enable_level_ends_if_killed",  Enable_Level_Ends_If_Killed},
	{"get_color",                    Get_Color},
	{"get_downgrade_count",          Get_Downgrade_Count},
	{"get_max_downgrade_count",      Get_Max_Downgrade_Count},
	{"is_boss",                      Is_Boss},
	{"on_downgrade",                 LUA_EVENT_HANDLER(downgrade)},
	{"set_color",                    Set_Color},
	{"set_max_downgrade_count",      Set_Max_Downgrade_Count},
	{NULL, NULL}
};

void Script::Open_Furball(lua_State* p_state)
{
	LuaWrap::register_subclass<cFurball>(p_state,
	                                   "Furball",
	                                   "Enemy",
	                                   Methods,
	                                   NULL,
	                                   Allocate,
	                                   NULL); // Memory managed by SMC

	// Register the "__index" metamethod for Furball
	lua_getglobal(p_state, "Furball");
	lua_newtable(p_state);
	lua_pushstring(p_state, "__index");
	lua_pushcfunction(p_state, Sprite___Index<cEnemy>);
	lua_settable(p_state, -3);
	lua_setmetatable(p_state, -2);
	lua_pop(p_state, 1); // Remove the class table for balancing
}
