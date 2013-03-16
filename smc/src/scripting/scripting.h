// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPTING_HPP
#define SMC_SCRIPTING_HPP
#include <string>
#include <mruby.h>
#include <map>
#include <mruby/compile.h>
#include <mruby/string.h>
#include <mruby/hash.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include <mruby/proc.h>

#include "../core/global_game.h"

// Some defines to ease use of mruby
#define MRB_ARGUMENT_ERROR(mrb) (mrb_class_obj_get(mrb, "ArgumentError"))
#define MRB_RUNTIME_ERROR(mrb) (mrb_class_obj_get(mrb, "RuntimeError"))

namespace SMC {
	namespace Scripting {

		class cMRuby_Interpreter {
		public:
			cMRuby_Interpreter(cLevel* p_level);
			~cMRuby_Interpreter();

			bool Run_Code(const std::string& code, std::string& errormsg);

			mrb_state* Get_MRuby_State();
			cLevel* Get_Level();
		private:
			mrb_state* mp_mruby;
			cLevel* mp_level;

			void Init_SMC_Libs();
		};
	};
};

#endif
