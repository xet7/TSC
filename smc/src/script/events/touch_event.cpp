#include <string>
#include "../luawrap.hpp"
#include "../../objects/sprite.h"
#include "touch_event.h"

namespace SMC{
	namespace Script{

		cTouch_Event::cTouch_Event(cSprite* p_collided)
		{
			mp_collided = p_collided;
		}

		std::string cTouch_Event::Event_Name()
		{
			return "touch";
		}

		cSprite* cTouch_Event::Get_Collided()
		{
			return mp_collided;
		}

		int cTouch_Event::Run_Lua_Callback(lua_State* p_state)
		{
			// Wrap the sprite object into a Lua object
			lua_getglobal(p_state, "Sprite"); // Class table needed for the instance method table
			cSprite** pp_sprite = (cSprite**) lua_newuserdata(p_state, sizeof(cSprite*));
			*pp_sprite					= mp_collided;
			// Attach the instance method table
			LuaWrap::InternalC::set_imethod_table(p_state);
			// Remove the class table
			lua_insert(p_state, -2);
			lua_pop(p_state, 1);

			// Call the handler
			return lua_pcall(p_state, 1, 0, 0);
		}

	};
};
