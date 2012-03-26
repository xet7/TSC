#include "luawrap.hpp"
#include "lua_level_player.h"
#include "../level/level_player.h"

namespace SMC
{
	namespace Script
	{

		static int Downgrade(lua_State* p_state)
		{
			if (!lua_istable(p_state, 1))
				return luaL_error(p_state, "No singleton table given.");

			pLevel_Player->DownGrade();
			return 0;
		}

		static luaL_Reg Player_Methods[] = {
			{"downgrade", Downgrade},
			{NULL, NULL}
		};

		void Open_Level_Player(lua_State* p_state)
		{
			LuaWrap::register_singleton(p_state, "Player", Player_Methods);
		}

	};
};
