#include "shoot_event.hpp"

using namespace SMC;
using namespace SMC::Scripting;

cShoot_Event::cShoot_Event(string ball_type)
{
	m_ball_type = ball_type;
}

string cShoot_Event::Event_Name()
{
	return "shoot";
}

string cShoot_Event::Get_Ball_Type()
{
	return m_ball_type;
}

void cShoot_Event::Run_MRuby_Callback(cMRuby_Interpreter* p_mruby, mrb_value callback)
{
	mrb_funcall(p_mruby->Get_MRuby_State(),
				callback,
				"call",
				1,
				str2sym(p_mruby->Get_MRuby_State(), (m_ball_type.c_str())));
}
