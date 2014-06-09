#include "scriptable_object.hpp"

using namespace SMC;
using namespace SMC::Scripting;

cScriptable_Object::cScriptable_Object()
{
	//
}

cScriptable_Object::~cScriptable_Object()
{
	clear_event_handlers();
}

/**
 * Clear all event handlers for all events. This is necessary when
 * you wipe out the mruby interpreter and set up a new one, as does
 * cLevel::Reinitialize_MRuby_Interpreter(), because deleting the
 * mruby interpreter automatically invalidates any mruby object
 * reference (`mrb_value`) still flying around. If you don’t clear
 * the event handler list (which precisely holds `mrb_values`) you
 * will get unpredictable behaviour reaching from mruby exceptions
 * to segmentation faults when an event gets fired for which an
 * event handler has been registered prior to deleting the mruby
 * interpreter.
 */
void cScriptable_Object::clear_event_handlers()
{
	m_callbacks.clear();
}

/**
 * Register a new event handler for an event, adding to the list of already
 * existing event handlers (if any).
 *
 * \param evtname
 *   Name of the event to register for. This has to match a return value
 *   of a cEvent::Event_Name() override (otherwise the handler will
 *   never get executed).
 * \param callback
 *   An mruby proc object to be executed when the event gets fired.
 */
void cScriptable_Object::register_event_handler(const std::string& evtname, mrb_value callback)
{
	m_callbacks[evtname].push_back(callback);
}

/**
 * Start iterator for the list of callbacks registered for the
 * given event name.
 *
 * \param evtname Name of the event you want the handlers for.
 *
 * \returns Iterator pointing to the first callback.
 */
std::vector<mrb_value>::iterator cScriptable_Object::event_handlers_begin(const std::string& evtname)
{
	return m_callbacks[evtname].begin();
}

/**
 * Stop iterator for the list of callbacks registered for the
 * given event name.
 *
 * \param evtname Name of the event you want the handlers for.
 *
 * \returns Iterator pointing post the last callback (termination iterator).
 */
std::vector<mrb_value>::iterator cScriptable_Object::event_handlers_end(const std::string& evtname)
{
	return m_callbacks[evtname].end();
}
