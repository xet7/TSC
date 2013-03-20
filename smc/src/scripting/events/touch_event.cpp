// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "touch_event.h"
#include "../objects/mrb_uids.h"
#include "../../objects/sprite.h"

using namespace SMC;
using namespace SMC::Scripting;

cTouch_Event::cTouch_Event(cSprite* p_collided)
{
	mp_collided = p_collided;
}

std::string cTouch_Event::Event_Name()
{
	return "touch";
}

cSprite* cTouch_Event::Get_Collided()
{
	return mp_collided;
}

void cTouch_Event::Run_MRuby_Callback(cMRuby_Interpreter* p_mruby, mrb_value callback)
{
	mrb_state* p_state = p_mruby->Get_MRuby_State();

	// Look the UID of the colliding sprite up in the UIDS table
	// and grab the MRuby object representing it.
	mrb_value rcollided = mrb_funcall(p_state,
									  mrb_obj_value(p_rmUIDS),
									  "[]",
									  1,
									  mrb_fixnum_value(mp_collided->m_uid));
	// Pass it to the callback
	mrb_funcall(p_state, callback, "call", 1, rcollided);
}
