// -*- c++ -*-
#ifndef SMC_LUA_EVENTS_H
#define SMC_LUA_EVENTS_H

#include "luawrap.hpp"
#include "../objects/animated_sprite.h"

namespace SMC{
	namespace Script{
		class cEvent
		{
		public:
			static void Register_Handler(int registryindex);
			void Fire(lua_State* p_state);
		private:
			static std::vector<int> M_handlers;
			int Run_Lua_Callback(lua_State* p_state);
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

		class cKeyDownEvent: public cEvent
		{
		public:
			cKeyDownEvent(std::string keyname);
			std::string Get_Keyname();
		private:
			std::string m_keyname;
		};

		void Open_Events(lua_State* p_state);
	};
};
#endif
