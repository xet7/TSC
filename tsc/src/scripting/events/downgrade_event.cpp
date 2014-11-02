#include "downgrade_event.hpp"

using namespace TSC;
using namespace TSC::Scripting;

cDowngrade_Event::cDowngrade_Event(int downgrades, int max_downgrades)
{
    m_downgrades = downgrades;
    m_max_downgrades = max_downgrades;
}

std::string cDowngrade_Event::Event_Name()
{
    return "downgrade";
}

int cDowngrade_Event::Get_Downgrades()
{
    return m_downgrades;
}

int cDowngrade_Event::Get_Max_Downgrades()
{
    return m_max_downgrades;
}

void cDowngrade_Event::Run_MRuby_Callback(cMRuby_Interpreter* p_mruby, mrb_value callback)
{
    mrb_funcall(p_mruby->Get_MRuby_State(),
                callback,
                "call",
                2,
                mrb_fixnum_value(m_downgrades),
                mrb_fixnum_value(m_max_downgrades));
}
