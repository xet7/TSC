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
		class cTimer
		{
		public:
			/* Constructor. Pass the Lua interpreter state to register
			 * the timer for, the time you want the timer
			 * to fire (in milliseconds) and the Lua registry
			 * index of the intended callback to register for firing.
			 * If `is_periodic' is true, the timer loops instead of
			 * just waiting a single time. */
			cTimer(cLua_Interpreter* p_lua, unsigned int interval, int reg_index, bool is_periodic = false);
			~cTimer();

			// Start ticking, the timer does nothing until
			// you call this. You can start a timer again
			// after you called Stop() (this applies to
			// periodic timers as well). Does nothing if the
			// timer is already running.
			void Start();
			// Stop ticking to the next possible time. The
			// callback may be run a final time after calling
			// this; blocks until the timer has stopped. Does
			// nothing if the timer has already been stopped.
			void Stop();
			// Returns true if the timer shall terminate
			// as soon as possible.
			bool Shall_Halt();
			// Returns true if the timer is running, i.e.
			// Start() has been called, but not Stop(). Note
			// this also returns true if a one-shot timer
			// has already finished, but Stop() has not been
			// called on it.
			bool Is_Active();

			// Attribute getters
			bool				Is_Periodic();
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
			static void Threading_Function(cTimer* timer);

			// True if this is a repeating timer.
			bool			m_is_periodic;
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
