#ifndef SMC_LUA_EVENTS_H
#define SMC_LUA_EVENTS_H

#include "luawrap.hpp"
#include "../objects/animated_sprite.h"

namespace SMC{
	namespace Script{
		class Event
		{
		public:
			static void register_handler(int registryindex);
			void fire(lua_State* p_state);
		private:
			static std::vector<int> M_handlers;
			int run_lua_callback(lua_State* p_state);
		};

		/*class TouchEvent: public Event
		{
		public:
			TouchEvent(cAnimatedSprite* p_collider1, cAnimatedSprite* p_collider2);
			cAnimatedSprite* get_collider1();
			cAnimatedSprite* get_collider2();
		private:
			cAnimatedSprite* mp_collider1;
			cAnimatedSprite* mp_collider2;
			};*/

		class KeyDownEvent: public Event
		{
		public:
			KeyDownEvent(std::string keyname);
			std::string get_keyname();
		private:
			std::string m_keyname;
		};

		void open_events(lua_State* p_state);
	};
};
#endif
