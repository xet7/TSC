// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "mrb_timer.h"

using namespace SMC;
using namespace Scripting;

static void Free_Timer(mrb_state* p_state, void* ptr);

// Extern
struct SMC::Scripting::p_rcTimer = NULL;
struct mrb_data_type SMC::Scripting::rtTimer = {"Timer", Free_Timer};

/***************************************
 * C++ part
 ***************************************/

// Note this method is ever and only called from Timer.new on the
// Mruby side, hence we don’t need to secure `callback' for the GC
// here. This is already done in Timer.new.
cTimer::cTimer(cMRuby_Interpreter* p_mruby, unsigned int interval, mrb_value callback, bool is_periodic /* = false */)
{
	mp_mruby			= p_mruby;
	m_interval			= interval;
	m_is_periodic		= is_periodic;
	m_callback			= callback;
	m_halt				= false;
	mp_thread			= NULL;
}

Script::cTimer::~cTimer()
{
	// If the timer is ticking currently, stop it.
	// This automatically deletes the thread.
	if (mp_thread)
		Stop();
}

void Script::cTimer::Start()
{
	if (mp_thread)
		return;

	m_halt = false;
	mp_thread = new boost::thread(Threading_Function, this);
}

void Script::cTimer::Stop()
{
	if (!mp_thread)
		return;

	m_halt = true;
	mp_thread->join();
	delete mp_thread;
}

bool Script::cTimer::Shall_Halt()
{
	return m_halt;
}

bool Script::cTimer::Is_Active()
{
	return !!mp_thread;
}

bool Script::cTimer::Is_Periodic()
{
	return m_is_periodic;
}

unsigned int Script::cTimer::Get_Interval()
{
	return m_interval;
}

boost::thread* Script::cTimer::Get_Thread()
{
	return mp_thread;
}

mrb_value Script::cTimer::Get_Callback()
{
	return m_callback;
}

cMRuby_Interpreter* Script::cTimer::Get_MRuby_Interpreter()
{
	return mp_mruby;
}

void Script::cTimer::Threading_Function(Script::cTimer* timer)
{
	if (timer->Is_Periodic()){
		while (true){
			boost::this_thread::sleep_for(boost::chrono::milliseconds(timer->Get_Interval()));

			// End the timer if requested
			if (timer->Shall_Halt())
				break;

			timer->Get_MRuby_Interpreter()->Register_Callback(timer->Get_Callback()); // This method is threadsafe
		}
	}
	else{ // One-shot timer
		boost::this_thread::sleep_for(boost::chrono::milliseconds(timer->Get_Interval()));

		// Don’t execute the callback anymore if halting was requested
		if (timer->Shall_Halt())
			return;

		timer->Get_MRuby_Interpreter()->Register_Callback(timer->Get_Callback());
	}
}

/***************************************
 * MRuby side
 ***************************************/

/**
 * Method: Timer::new
 *
 *   new( interval [, is_periodic ] ){...} → a_timer
 *
 * TODO: Docs.
 */
static mrb_value Initialize(mrb_state* p_state,  mrb_value self)
{
	mrb_int interval;
	mrb_value is_periodic = mrb_false_value();
	mrb_value block;
	mrb_get_args(p_state, "i|o&" &interval, &is_periodic, &block);

	// The cTimer constructor needs the currently active cMRuby_Interpreter
	// instance which is not reachable via the mrb_state*, hence we retrieve
	// it from the currently active level which at this point is always the
	// same MRuby instance as `p_state'.
	cTimer* p_timer = new cTimer(pActive_Level->Get_MRuby_Interpreter(), interval, block, mrb_test(is_periodic));
	DATA_PTR(self) = p_timer;
	DATA_TYPE(self) = rtTimer;

	// Prevent the GC from collecting the objects by a) adding ourselves
	// to the class-instance variable instances and b) adding the callback
	// to ourselves (the GC doesn’t see the reference in the C++ cTimer
	// instance).
	mrb_value klass = mrb_obj_value(mrb_obj_class(p_state, self));
	mrb_ary_push(p_state, mrb_iv_get(p_state, klass, mrb_intern("instances")), self);
	mrb_iv_set(p_state, self, mrb_intern(p_state, "callback"), block);
}

static void Free_Timer(mrb_state* p_state, void* ptr)
{
	cTimer* p_timer = (cTimer*) ptr;
	delete p_timer;
}

/**
 * Method: Timer#start
 *
 *   start()
 *
 * TODO: Docs.
 */
static mrb_value Start(mrb_state* p_state,  mrb_value self)
{
	cTimer* p_timer = Get_Data_Ptr<cTimer>(p_state, self);

	p_timer->Start();
	return mrb_nil_value();
}

/**
 * Method: Timer#stop
 *
 *   stop()
 *
 * TODO: Docs.
 */
static mrb_value Stop(mrb_state* p_state,  mrb_value self)
{
	cTimer* p_timer = Get_Data_Ptr<cTimer>(p_state, self);

	p_timer->Stop();
	return mrb_nil_value();
}

void SMC::Scripting::Init_Timer(mrb_state* p_state)
{
	p_rcTimer = mrb_define_class(p_state, "Timer", p_state->object_class);
	mrb_include_module(p_state, p_rcTimer, p_rmEventable);
	MRB_SET_INSTANCE_TT(p_rcTimer, MRB_TT_DATA);

	// Invisible (for MRuby) class instance variable for storing the
	// Timer instances so they don’t get GC’ed.
	mrb_iv_set(p_state, mrb_obj_value(p_rcTimer), mrb_intern(p_state, "instances"), mrb_ary_new(p_state));

	mrb_define_method(p_state, p_rcTimer, "initialize", Initialize, ARGS_REQ(1) | ARGS_OPT(1) | ARGS_BLOCK());
	mrb_define_method(p_state, p_rcTimer, "start", Start, ARGS_NONE());
	mrb_define_method(p_state, p_rcTimer, "stop", Stop, ARGS_NONE());
}
