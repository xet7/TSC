// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/string.h>
#include <mruby/hash.h>

#include "../level/level.h"
#include "../core/sprite_manager.h"

#include "scripting.h"

namespace SMC
{

	namespace Scripting
	{

		cMRuby_Interpreter::cMRuby_Interpreter(cLevel* p_level)
		{
			// Set member variables
			mp_level = p_level;
			mp_mruby = mrb_open();

			// Load our extensions into mruby
			Init_SMC_Libs();

			// Build up the UID table
			m_uid_table = mrb_hash_new(mp_mruby);
			mrb_define_const(mp_mruby, mp_mruby->object_class, "UIDS", m_uid_table);
		}

		cMRuby_Interpreter::~cMRuby_Interpreter()
		{
			mrb_close(mp_mruby);
		}

		mrb_state* cMRuby_Interpreter::Get_MRuby_State()
		{
			return mp_mruby;
		}

		cLevel* cMRuby_Interpreter::Get_Level()
		{
			return mp_level;
		}

		bool cMRuby_Interpreter::Run_Code(const std::string& code, std::string& errormsg)
		{
			mrb_load_string(mp_mruby, code.c_str());
			if (mp_mruby->exc){
				// Format an exception method like this: Message (exception class)
				errormsg = std::string(mrb_string_value_ptr(mp_mruby, mrb_funcall(mp_mruby, mrb_obj_value(mp_mruby->exc), "message", 0)));
				errormsg.append(" (");
				errormsg.append(mrb_obj_classname(mp_mruby, mrb_obj_value(mp_mruby->exc)));
				errormsg.append(")");
				return false;
			}
			else
				return true;
		}

		void cMRuby_Interpreter::Init_SMC_Libs()
		{

		}

	};

};
