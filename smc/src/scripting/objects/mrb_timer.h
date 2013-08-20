// -*- c++ -*-
#ifndef SMC_SCRIPTING_TIMER_H
#define SMC_SCRIPTING_TIMER_H
#include <boost/thread/thread.hpp>
#include "../scripting.h"

namespace SMC {
	namespace Scripting {

		// C++ side of the MRuby Timer class.
		class cTimer
		{
		public:
			/* Constructor. Pass the MRuby interpreter state to register
			 * the timer for, the time you want the timer
			 * to fire (in milliseconds) and the callback to register for firing.
			 * If `is_periodic' is true, the timer loops instead of
			 * just waiting a single time. */
			cTimer(cMRuby_Interpreter* p_mruby, unsigned int interval, mrb_value callback, bool is_periodic = false);
			~cTimer();

			// Start ticking, the timer does nothing until
			// you call this. You can start a timer again
			// after you called Stop() (this applies to
			// periodic timers as well). Does nothing if the
			// timer is already running.
			void Start();
			// Stop ticking to the next possible time. The
			// callback; blocks until the timer has stopped. Does
			// nothing if the timer has already been stopped.
			// You must call this for a oneshot-timer even if
			// it already fired if you want to restart it.
			void Stop();
			// Returns true if the timer shall terminate
			// as soon as possible.
			bool Shall_Halt();
			// Returns true if the timer is running currently.
			// This may still return true if a call to Stop()
			// has not yet been honoured.
			bool Is_Active();
			// Marks the timer as stopped. This is private API,
			// don’t use this.
			void Set_Stopped();

			// Attribute getters
			bool				Is_Periodic();
			unsigned int		Get_Interval();
			boost::thread*		Get_Thread();
			mrb_value			Get_Callback();
			cMRuby_Interpreter*	Get_MRuby_Interpreter();
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
			// The callback to register.
			mrb_value		m_callback;
			// The thread. Only set after calling start().
			boost::thread*	mp_thread;
			// The MRuby instance we’re attaching the callbacks to.
			cMRuby_Interpreter* mp_mruby;
			// If set, stops the timer as soon as possible.
			bool m_halt;
			// If set, the auxiliary thread is not running.
			bool m_stopped;
		};

		extern struct RClass* p_rcTimer;
		extern mrb_data_type rtTimer;

		// Usual function for initialising the binding
		void Init_Timer(mrb_state* p_state);
	}
}

#endif
