#include "luawrap.hpp"
#include "../objects/animated_sprite.h"
#include "events.h"

namespace SMC
{
	namespace Script
	{

		/* *** *** *** *** *** Event *** *** *** *** *** *** *** *** *** *** *** *** */

		// Static member definition
		std::vector<int>Event::M_handlers;

		/**
		 * Registers a Lua callback function for this event.
		 * registryindex is the index of the function in the
		 * Lua registry which is created in the function
		 * binding this function to Lua.
		 */
		void Event::register_handler(int registryindex)
		{
			M_handlers.push_back(registryindex);
		}

		/**
		 * Cycles through all registered event handlers and
		 * calls the instance method Event::run_lua_callback()
		 * for them. See run_lua_callback()’s documentation for
		 * more information on this.
		 *
		 * For subclasses, you usually don’t want to overwrite fire(),
		 * but rather run_lua_callback().
		 */
		void Event::fire(lua_State* p_state)
		{
			for(std::vector<int>::iterator iter = M_handlers.begin(); iter < M_handlers.end(); iter++){
				lua_rawgeti(p_state, LUA_REGISTRYINDEX, *iter);
				if (run_lua_callback(p_state) != LUA_OK){
					std::cerr << "Error running Lua handler: " << lua_tostring(p_state, -1);
					lua_pop(p_state, 1); // Remove the error message from the stack
				}
			}
		}

		/**
		 * Called whenever a Lua callback function shall be run.
		 * When this is called, the callback Lua function already
		 * is on the stack at index -1, i.e. at the top. It should
		 * forward the result of the call to lua_pcall() as the return
		 * value.
		 *
		 * The default implementation in the Event class just calls
		 * the function without any arguments.
		 */
		int Event::run_lua_callback(lua_State* p_state)
		{
			return lua_pcall(p_state, 0, 0, 0);
		}

		/**
		 * This method is the event classes’ register() classmethod
		 * from Lua. It expects a Lua function as the last argument
		 * and registers it in the list of handlers for the given
		 * event class T. T must be a subclass of Event.
		 */
		template<typename T>
		static int lua_event_register(lua_State* p_state)
		{
			if (!lua_istable(p_state, 1))
				return luaL_error(p_state, "No class table given.");
			if (!lua_isfunction(p_state, -1))
				return luaL_error(p_state, "No callback function given.");

			// Copy the function to the top of the stack
			// (we don’t want to remove what’s handed to us)
			lua_pushvalue(p_state, -1);

			// Add the function to the list of registered handlers
			T::register_handler(luaL_ref(p_state, LUA_REGISTRYINDEX));

			return 0;
		}

		/* *** *** *** *** *** KeyDownEvent *** *** *** *** *** *** *** *** *** *** *** *** */

		/***************************************
		 * C++ implementation
		 ***************************************/

		KeyDownEvent::KeyDownEvent(std::string keyname)
		{
			m_keyname = keyname;
		}

		std::string KeyDownEvent::get_keyname()
		{
			return m_keyname;
		}

		/***************************************
		 * Lua binding
		 ***************************************/

		static int lua_key_down_event_alloc(lua_State* p_state)
		{
			if (!lua_istable(p_state, 1))
				return luaL_error(p_state, "No class table given.");

			std::string keyname = luaL_checkstring(p_state, 2);
			KeyDownEvent* p_event = new (lua_newuserdata(p_state, sizeof(KeyDownEvent))) KeyDownEvent(keyname);

			if (!p_event)
				return luaL_error(p_state, "Failed to allocate memory.");

			return 1;
		}

		static int lua_key_down_get_keyname(lua_State* p_state)
		{
			KeyDownEvent* p_event = LuaWrap::to<KeyDownEvent>(p_state);
			lua_pushstring(p_state, p_event->get_keyname().c_str());
			return 1;
		}

		static luaL_Reg key_down_event_cmethods[] = {
			{"register", lua_event_register<KeyDownEvent>},
			{NULL, NULL}
		};

		static luaL_Reg key_down_event_imethods[] = {
			{"keyname", lua_key_down_get_keyname},
			{NULL, NULL}
		};

		/* *** *** *** *** *** namespace functions *** *** *** *** *** *** *** *** *** *** *** *** */

		void open_events(lua_State* p_state)
		{
			LuaWrap::register_class<KeyDownEvent>(p_state, "KeyDownEvent", key_down_event_imethods, key_down_event_cmethods, lua_key_down_event_alloc);
		}

	};
};
