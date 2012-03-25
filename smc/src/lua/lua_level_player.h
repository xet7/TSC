#ifndef SMC_LUA_LEVEL_PLAYER_H
#define SMC_LUA_LEVEL_PLAYER_H

#include "luawrap.hpp"
#include "../level/level_player.h"

namespace SMC
{
	namespace Lua
	{
		void open_level_player(lua_State* p_state);
	};
};

#endif
