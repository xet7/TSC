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
#include <boost/filesystem.hpp>

#include "../core/global_game.h"

// Some defines to ease use of mruby
#define MRB_ARGUMENT_ERROR(mrb) (mrb_class_obj_get(mrb, "ArgumentError"))
#define MRB_RUNTIME_ERROR(mrb) (mrb_class_obj_get(mrb, "RuntimeError"))

namespace SMC {
	namespace Scripting {

		extern boost::filesystem::path scripting_dir;

		class cMRuby_Interpreter {
		public:
			// Create a new MRuby instance for the given level.
			cMRuby_Interpreter(cLevel* p_level);
			// Destructor
			~cMRuby_Interpreter();

			// Execute MRuby code. If an exception occurs
			// (including syntax errors), `errormsg' will
			// contain a human-readable description and
			// false is returned, true otherwise.
			bool Run_Code(const std::string& code, std::string& errormsg);

			// Returns the underlying mrb_state*.
			mrb_state* Get_MRuby_State();
			// Returns the cLevel* we’re associated with.
			cLevel* Get_Level();
		private:
			mrb_state* mp_mruby;
			cLevel* mp_level;

			// Does basic setup and then executes the main.rb file.
			void Load_Scripts();
		};
	};
};

#endif
