// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "event.h"
#include "../../core/property_helper.h"

using namespace SMC;
using namespace SMC::Scripting;

/**
 * Cycles through all registered event handlers for the event
 * name returned by the Event_Name() method and calls the
 * Run_MRuby_Callback() method for each of them. See Run_MRuby_Callback()’s
 * documentation for more information on this.
 *
 * For subclasses, you don’t want to override Fire(), but rather
 * Run_Lua_Callback() and Event_Name().
 */
void cEvent::Fire(cMRuby_Interpreter* p_mruby, Script::cScriptable_Object* p_obj)
{
	// Menu level has no mruby interpreter
	if (!p_mruby)
		return;
	mrb_state* p_state = p_mruby->Get_MRuby_State();

	// Iterate through the list of callbacks and execute them
	std::vector<mrb_value> callbacks = p_obj->m_callbacks[Event_Name()];
	std::vector<mrb_value>::iterator iter;
	for(iter = callbacks.begin(); iter != callbacks.end(); iter++){
		Run_MRuby_Callback(p_mruby, *iter);
		if (p_state->exc)
			std::cerr << "Warning: Error running mruby handler: " << format_mruby_error(p_state, p_state->exc) << std::endl;
	}
}

/**
 * Returns the name of the event, used for determining which
 * callbacks to run when Fire() is called. Subclasses must override
 * this method in order to return a proper name identifying them
 * uniquely among all possible event names. The names returned
 * by all the Event_Name() functions must correspond to the names
 * you pass to the MRUBY_IMPLEMENT_EVENT and MRUBY_EVENT_HANDLER
 * macros that implement the "on_*" methods.
 */
std::string cEvent::Event_Name()
{
	return "generic";
}

/**
 * Called whenever a MRuby callback shall be run. The callback is
 * passed as a mruby lambda via the `callback' argument.
 *
 * This method is intended to be overridden in more specific event classes,
 * that should however eventually call the #call() method on the lambda object.
 * The default implemention of this method just does exactly that, without
 * passing any further arguments or inspecting the return value of the callback.
 *
 * Leave p_mruby->Get_MRuby_State()->exc set to get that exception automatically
 * printed out to cerr (via the Fire() method that calls this method).
 */
void cEvent::Run_MRuby_Callback(cMRuby_Interpreter* p_mruby, mrb_value callback)
{
	mrb_funcall(p_mruby->Get_MRuby_State(), callback, "call", 0);
}
