// -*- c++ -*-
#ifndef SMC_LUA_LUALIBS_H
#define SMC_LUA_LUALIBS_H

#include "luawrap.hpp"
#include "lua_level_player.h"
#include "events.h"
#include "lualibs.h"

namespace SMC
{
	namespace Script
	{
		void openlibs(lua_State* p_state);
	};
};

#endif
