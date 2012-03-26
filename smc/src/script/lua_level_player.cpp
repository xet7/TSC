#include "luawrap.hpp"
#include "lua_level_player.h"
#include "../level/level_player.h"

namespace SMC
{
	namespace Script
	{

		static int downgrade(lua_State* p_state)
		{
			if (!lua_istable(p_state, 1))
				return luaL_error(p_state, "No singleton table given.");

			pLevel_Player->DownGrade();
			return 0;
		}

		static luaL_Reg player_methods[] = {
			{"downgrade", downgrade},
			{NULL, NULL}
		};

		void open_level_player(lua_State* p_state)
		{
			LuaWrap::register_singleton(p_state, "Player", player_methods);
		}

	};
};
