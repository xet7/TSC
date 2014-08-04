#include "key_down_event.hpp"

using namespace SMC;
using namespace SMC::Scripting;

cKeyDown_Event::cKeyDown_Event(string keyname)
{
	m_keyname = keyname;
}

string cKeyDown_Event::Get_Keyname()
{
	return m_keyname;
}

string cKeyDown_Event::Event_Name()
{
	return "key_down";
}

void cKeyDown_Event::Run_MRuby_Callback(cMRuby_Interpreter* p_mruby, mrb_value callback)
{
	mrb_state* p_state = p_mruby->Get_MRuby_State();
	mrb_funcall(p_state,
				callback,
				"call",
				1,
				mrb_str_new_cstr(p_state, m_keyname.c_str()));
}
