// -*- c++ -*-
#ifndef SMC_SCRIPT_EVENTS_EVENT_H
#define SMC_SCRIPT_EVENTS_EVENT_H
#include <vector>
#include "../luawrap.hpp"

namespace SMC{
	namespace Script{
		class cEvent
		{
		public:
			static void Register_Handler(int registryindex);
			void Fire(lua_State* p_state);
		private:
			static std::vector<int> M_handlers;
			virtual int Run_Lua_Callback(lua_State* p_state);
		};

		/**
		 * This method is the event classes’ register() classmethod
		 * from Lua. It expects a Lua function as the last argument
		 * and registers it in the list of handlers for the given
		 * event class T. T must be a subclass of cEvent.
		 */
		template<typename T>
		int lua_event_register(lua_State* p_state)
		{
			if (!lua_istable(p_state, 1))
				return luaL_error(p_state, "No class table given.");
			if (!lua_isfunction(p_state, -1))
				return luaL_error(p_state, "No callback function given.");

			// Copy the function to the top of the stack
			// (we don’t want to remove what’s handed to us)
			lua_pushvalue(p_state, -1);

			// Add the function to the list of registered handlers
			T::Register_Handler(luaL_ref(p_state, LUA_REGISTRYINDEX));

			return 0;
		}
	};
};
#endif
