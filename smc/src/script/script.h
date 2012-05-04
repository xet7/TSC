// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPT_H
#define SMC_SCRIPT_H
#include <typeinfo>
#include <string>
#include <map>
#include "luawrap.hpp"
#include "../core/global_game.h"

namespace SMC
{
	namespace Script
	{
		// Set up basic things needed for any scripting
		void		Initialize_Scripting();
		// Create a new Lua interprter and initialise it.
		// level: The level for which to initialise the interpeter.
		//        Note this doesn’t set m_lua on the level, it’s merely
		//        used to get the UID table from the level’s sprite list.
		lua_State	*New_Lua_State(const cLevel* p_level);
		// Destroy the given Lua interpreter.
		void		Cleanup_Lua_State(lua_State* p_state);

		// This table maps C++ class names (type_info.name()) to Lua class names.
		typedef std::map<std::string, std::string> ClassMap;

		// Maps C++ class names to Lua class names
		extern ClassMap type2class;
		// Maps Lua class names to C++ class names.
		extern ClassMap class2type;
	};
};

#endif
