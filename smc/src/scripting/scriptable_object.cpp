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

void cScriptable_Object::clear_event_handlers()
{
	m_callbacks.clear();
}

void cScriptable_Object::register_event_handler(const std::string& evtname, mrb_value callback)
{
	m_callbacks[evtname].push_back(callback);
}

std::vector<mrb_value>::iterator cScriptable_Object::event_handlers_begin(const std::string& evtname)
{
	return m_callbacks[evtname].begin();
}

std::vector<mrb_value>::iterator cScriptable_Object::event_handlers_end(const std::string& evtname)
{
	return m_callbacks[evtname].end();
}
