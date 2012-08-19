// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPT_H
#define SMC_SCRIPT_H
#include <typeinfo>
#include <string>
#include <map>
#include "luawrap.hpp"
#include "../core/global_game.h"

namespace SMC{
	namespace Script{
		class cLua_Interpreter{
		public:
			// Attach a Lua interpreter to the given level and initialise it.
			cLua_Interpreter(cLevel* p_level);
			~cLua_Interpreter();

			// Run arbitrary Lua code. Returns true on success. Otherwise
			// returns false at sets `errormsg' to the Lua error message.
			bool Run_Code(const std::string& code, std::string& errormsg);
			// Wrap `sprite' in a Lua object and add it to the Lua-global
			// UID table.
			void Add_To_UID_Table(cSprite& sprite);

			// The underlying lua_State* provided by the Lua library.
			lua_State* Get_Lua_State();
			// The level this interpreter is attached to.
			cLevel* Get_Level();
		private:
			lua_State*	mp_lua;
			cLevel*		mp_level;

			// Loads the SMC-specific Lua libraries into mp_lua.
			void Open_SMC_Libs();

			// Creates a Lua full userdata object for `sprite' and adds it
			// to the top of the Lua stack. [+1 on mp_lua stack]
			void Wrap_Lua_Object_Around_Sprite(cSprite& sprite);
		};

		// Name of the Lua global table mapping the UIDs to
		// objects of Sprite and its subclasses.
		const std::string UID_TABLE_NAME = "UIDS";

		// Set up basic things needed for any scripting
		void Initialize_Scripting();

		// This table maps C++ class names (type_info.name()) to Lua class names.
		typedef std::map<std::string, std::string> ClassMap;

		// Maps C++ class names to Lua class names
		extern ClassMap type2class;
		// Maps Lua class names to C++ class names.
		extern ClassMap class2type;
	};
};

#endif
