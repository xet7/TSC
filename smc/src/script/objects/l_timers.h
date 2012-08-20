// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPT_TIMER_H
#define SMC_SCRIPT_TIMER_H
#include <vector>
#include <boost/thread/thread.hpp>
#include "../script.h"
namespace SMC{
	namespace Script{

		/* HACK: script.h, which defines cLua_Interpreter, needs
		 * this header and vice-versa. Add this prototype to
		 * make the compiler not complain about cLua_Interpreter
		 * being undefined due to the circular include. */
		class cLua_Interpreter;

		// C++ side of the Lua PeriodicTimer class.
		class cPeriodic_Timer
		{
		public:
			/* Constructor. Pass the Lua interpreter state to register
			 * the timer for, the time you want the timer
			 * to fire (in milliseconds) and the Lua registry
			 * index of the intended callback to register for firing. */
			cPeriodic_Timer(cLua_Interpreter* p_lua, unsigned int interval, int reg_index);
			~cPeriodic_Timer();

			// Start ticking. The timer does nothing until
			// you call this.
			void Start();
			// Stop ticking to the next possible time. The
			// callback may be run a final time after calling
			// this; blocks until the timer has stopped.
			void Stop();
			// Returns true if the timer shall terminate
			// as soon as possible.
			bool Shall_Halt();

			// Attribute getters
			unsigned int		Get_Interval();
			boost::thread*		Get_Thread();
			int             	Get_Index();
			cLua_Interpreter*	Get_Lua_Interpreter();
		private:
			// This is the body of the thread used by the
			// timer. Contains an endless loop for waiting
			// and registering. Note one cannot use non-static
			// members as the thread body, hence this static-with-`this'-pointer
			// construct.
			static void Threading_Function(cPeriodic_Timer* timer);

			// Time interval.
			unsigned int	m_interval;
			// The index to register.
			int				m_registry_index;
			// The thread. Only set after calling start().
			boost::thread*	mp_thread;
			// The Lua instance we’re attaching the callbacks to.
			cLua_Interpreter* mp_lua;
			// If set, stops the timmer as soon as possible.
			bool m_halt;
		};

		// Usual function for opening the Lua binding.
		void Open_Timers(lua_State* p_state);
	};
};
#endif
