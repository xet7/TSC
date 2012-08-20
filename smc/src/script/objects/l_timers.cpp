// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include <vector>
#include <boost/thread/thread.hpp>
#include "../../level/level.h"
#include "../script.h"
#include "l_timers.h"

using namespace SMC;
using namespace SMC::Script;

/**** How the timers and their callbacks work ****
 * Creating an active timer is a two-step process. The first
 * step is to instanciate the respective timer class, which
 * remembers the values needed for adjusting the timer --
 * most importantly the waiting time and the callback. The
 * callback is a Lua function which is copied into the
 * Lua registry to easily reference it from anywhere; the
 * resulting index is then stored by the timer instance.
 * 
 * You then call the timer’s Start() method which creates a
 * new thread (boost::thread) that waits the time specified
 * by the timer initialisation. Rather than immediately exe-
 * cuting the callback in the thread’s context (which is a
 * very complex task due to Lua not having native multi-
 * threading facilities) it calls
 * Lua_Interpreter::Register_Callback_Index(), which adds
 * the callback’s index to a list of pending callback indices.
 * This list is iterated once a frame in cLevel::Update()
 * and executes the functions corresponding to the indices
 * from the Lua registry. After all pending callbacks have
 * been executed, the list is cleared and normal gameplay
 * resumes. This way the callbacks are executed synchronous
 * to the rest of the SMC and Lua stuff, while still allowing
 * the callback "injection" to remain asynchronous and
 * therefore high-precision. The payoff is that although the
 * actual "injection" is (nearly) asynchronous (there is a
 * mutex preventing race conditions around the list of
 * pending callback indices), the actual *execution* is
 * a bit delayed, as it will only happen when the normal
 * mainloop comes over cLevel::Update(), which is usually
 * once a frame for normal gameplay (i.e. not for
 * an active editor or the menu).
 *
 * Calling Stop() on a timer ends the underlying thread
 * as soon as possible, but not necessarily immediately,
 * because the termination condition is only checked every
 * now and then, so it may be that the timer registers
 * another callback call before actually finishing. The
 * thread is then deleted, and Stop() returns. If a timer
 * instance is deleted some way or another, it’s destructor
 * automatically calls Stop() for a running timer -- this
 * means your deletion process can take a while.
 *
 * The timers created from the Lua code a user supplies
 * are automatically (in their Lua allocation function)
 * registered with the calling Lua state using the
 * Lua_Interpreter::Register_Timer() method and are
 * deleted when the Lua state is closed. This allows
 * the C++ timer to go on ticking even if the referencing
 * Lua userdata object has gone out of scope. There is no
 * way to trigger the C++ timer’s deletion from the Lua
 * side except for ending the level. */

/***************************************
 * C++ part
 ***************************************/

Script::cPeriodic_Timer::cPeriodic_Timer(cLua_Interpreter* p_lua, unsigned int interval, int reg_index)
{
	mp_lua				= p_lua;
	m_interval			= interval;
	m_registry_index	= reg_index;
	m_halt				= true;
	mp_thread			= NULL;
}

Script::cPeriodic_Timer::~cPeriodic_Timer()
{
	// If the timer is ticking currently, stop it.
	// This automatically deletes the thread.
	if (mp_thread)
		Stop();
}

void Script::cPeriodic_Timer::Start()
{
	// TODO: Throw an exception if already started
	if (mp_thread)
		return;

	m_halt = false;
	mp_thread = new boost::thread(Threading_Function, this);
}

void Script::cPeriodic_Timer::Stop()
{
	// TODO: Throw an exception if not started
	if (!mp_thread)
		return;

	m_halt = true;
	mp_thread->join();
	delete mp_thread;
}

bool Script::cPeriodic_Timer::Shall_Halt()
{
	return m_halt;
}

unsigned int Script::cPeriodic_Timer::Get_Interval()
{
	return m_interval;
}

boost::thread* Script::cPeriodic_Timer::Get_Thread()
{
	return mp_thread;
}

int Script::cPeriodic_Timer::Get_Index()
{
	return m_registry_index;
}

cLua_Interpreter* Script::cPeriodic_Timer::Get_Lua_Interpreter()
{
	return mp_lua;
}

void Script::cPeriodic_Timer::Threading_Function(Script::cPeriodic_Timer* timer)
{
	while (true){
		// End the timer if requested
		if (timer->Shall_Halt())
			break;

		boost::this_thread::sleep_for(boost::chrono::milliseconds(timer->Get_Interval()));
		timer->Get_Lua_Interpreter()->Register_Callback_Index(timer->Get_Index()); // This method is threadsafe
	}
}

/***************************************
 * Lua part
 ***************************************/

static int Allocate(lua_State* p_state)
{
	if (!lua_istable(p_state, 1))
		return luaL_error(p_state, "No class table given.");
	if (!lua_isfunction(p_state, 3))
		return luaL_error(p_state, "Argument #3 is not a function.");
	unsigned int interval = static_cast<unsigned int>(luaL_checkunsigned(p_state, 2));

	/* Copy the function to the top of the stack
	 * (we don’t want to remove what’s handed to us)
	 * and store it in the registry. This is the callback
	 * that will be evaluated when the timer fires. */
	lua_pushvalue(p_state, 3);
	int index = luaL_ref(p_state, LUA_REGISTRYINDEX);

	cPeriodic_Timer**	pp_timer	= (cPeriodic_Timer**) lua_newuserdata(p_state, sizeof(cPeriodic_Timer*));
	cPeriodic_Timer*	p_timer		= new cPeriodic_Timer(pActive_Level->m_lua, interval, index); // Needs the Lua_Interpreter class
	*pp_timer						= p_timer;

	/* Note that the periodic timer created here isn’t deleted
	 * from the Lua side of things (i.e. garbage collection).
	 * This is due to the fact that the user most likely wants
	 * his timers to tick even after the initial timer variable
	 * has gone out of scope. So instead, the timers are deleted
	 * when their containing Lua state is closed. */
	pActive_Level->m_lua->Register_Timer(p_timer);

	LuaWrap::InternalC::set_imethod_table(p_state, 1); // Attach instance methods
	return 1;
}

static int Start(lua_State* p_state)
{
	cPeriodic_Timer* p_timer = *LuaWrap::check<cPeriodic_Timer*>(p_state, 1);
	p_timer->Start();
	return 0;
}

static int Stop(lua_State* p_state)
{
	cPeriodic_Timer* p_timer = *LuaWrap::check<cPeriodic_Timer*>(p_state, 1);
	p_timer->Stop();
	return 0;
}

static int Get_Interval(lua_State* p_state)
{
	cPeriodic_Timer* p_timer = *LuaWrap::check<cPeriodic_Timer*>(p_state, 1);
	lua_pushnumber(p_state, p_timer->Get_Interval());
	return 1;
}

/***************************************
 * Binding
 ***************************************/

static luaL_Reg Methods[] = {
	{"start",			Start},
	{"stop",			Stop},
	{"get_interval",	Get_Interval},
	{NULL, NULL}
};

void Script::Open_Timers(lua_State* p_state)
{
	LuaWrap::register_class<cPeriodic_Timer>(	p_state,
												"PeriodicTimer",
												Methods,
												NULL,
												Allocate,
												NULL);
}
