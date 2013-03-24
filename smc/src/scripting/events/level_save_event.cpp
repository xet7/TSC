// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "level_save_event.h"

using namespace SMC;
using namespace SMC::Scripting;

std::string cLevel_Save_Event::Event_Name()
{
	return "save";
}

std::string cLevel_Save_Event::Get_Save_Data()
{
	return m_save_data;
}

void cLevel_Save_Event::Run_MRuby_Callback(cMRuby_Interpreter* p_mruby, mrb_value callback)
{
	mrb_state* p_state = p_mruby->Get_MRuby_State();
	mrb_value mod_json = mrb_const_get(p_state, mrb_obj_value(p_state->object_class), mrb_intern(p_state, "JSON"));

	// Create a hash that can be filled by the callback
	mrb_value target_hsh = mrb_hash_new(p_state);
	mrb_funcall(p_state, callback, "call", 1, target_hsh);

	// Transform the hash to JSON (mruby doesn’t have Marshal, sadly).
	mrb_value mrb_result = mrb_funcall(p_state, mod_json, "stringify", 1, target_hsh);
	std::string result = mrb_string_value_ptr(p_state, mrb_result);

	// Make a copy of the string so the GC may collect the C pointer
	m_save_data = std::string(result);
}
