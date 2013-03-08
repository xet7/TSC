// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPTING_HPP
#define SMC_SCRIPTING_HPP
#include <string>
#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/string.h>
#include <mruby/hash.h>

#include "../core/global_game.h"

namespace SMC {
	namespace Scripting {

		const std::string UID_TABLE_NAME = "UIDS";

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
			mrb_value m_uid_table;

			void Init_SMC_Libs();

		};
	};
};

#endif
