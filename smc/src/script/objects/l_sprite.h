// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPT_SPRITE_H
#define SMC_SCRIPT_SPRITE_H
#include <string>
#include "../luawrap.hpp"
#include "../../level/level.h"
#include "../../core/sprite_manager.h"

// Defines a event handler function that just calls
// Forward_To_Register with the given argument.
#define LUA_IMPLEMENT_EVENT(evtname)\
	static int Script_Event_On_##evtname(lua_State* p_state){return SMC::Script::Forward_To_Sprite_Register(p_state, #evtname);}
// Expands to the name of the function defined
// by IMPLEMENT_LUA_EVENT.
#define LUA_EVENT_HANDLER(evtname) Script_Event_On_##evtname

namespace SMC{
	namespace Script{

		/**
		 * "__index" metamethod for the Lua Sprite classes. This method
		 * allows you to do things like
		 *	 Sprite[32]
		 * to get an instance of class Sprite with the sprite with UID 32.
		 */
		template<typename T>
		int Sprite___Index(lua_State* p_state)
		{
			// Get the UID
			int uid = static_cast<int>(luaL_checknumber(p_state, 2)); // TODO: unsigned long?

			/* Try to get the corresponding object. The dynamic_cast is necessary
			 * because Enemy[23] shouldn’t result in a bare sprite being returned
			 * while the user things it is an enemy (all sprites share the same
			 * UID system). */
			T* p_t = dynamic_cast<T*>(pActive_Level->m_sprite_manager->Get_by_UID(uid));

			if (!p_t) // Return nil if no T is found
				lua_pushnil(p_state);
			else{
				// Found, wrap the T into a Lua object
				lua_pushvalue(p_state, 1); // argument 1 is the class table
				T** pp_t	= (T**) lua_newuserdata(p_state, sizeof(T*));
				*pp_t		= p_t;

				// Attach instance methods
				LuaWrap::InternalC::set_imethod_table(p_state);

				// Remove the table
				lua_insert(p_state, -2);
				lua_pop(p_state, 1);
			}

			return 1; // Either nil or the Lua T object
		}

		int Forward_To_Sprite_Register(lua_State* p_state, std::string event_name);
		void Open_Sprite(lua_State* p_state);
	};
};

#endif
