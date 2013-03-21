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
#include <mruby/array.h>
#include <boost/filesystem.hpp>

#include "../core/global_game.h"

#include "objects/mrb_smc.h"

// Some defines to ease use of mruby
#define MRB_ARGUMENT_ERROR(mrb) (mrb_class_obj_get(mrb, "ArgumentError"))
#define MRB_RUNTIME_ERROR(mrb) (mrb_class_obj_get(mrb, "RuntimeError"))
#define MRB_TYPE_ERROR(mrb) (mrb_class_obj_get(mrb, "TypeError"))

namespace SMC {
	namespace Scripting {

		extern boost::filesystem::path scripting_dir;

		// Load all MRuby wrapper classes for the C++ classes
		// into the given mruby state. Called by SMC::setup
		// in mruby land.
		void Load_Wrappers(mrb_state* p_state);
		// Takes a C(++) string and directly returns an MRuby
		// symbol object (not an mrb_sym!) for it.
		inline mrb_value str2sym(mrb_state* mrb, std::string str){ return mrb_symbol_value(mrb_intern(mrb, str.c_str())); }

		// More permissive version of MRuby’s Data_Get_Struct(). This
		// function doesn’t check the type pointer, which is totally
		// useless for wrapping C++ class hierarchies, because MRuby
		// doesn’t recgonize a pointer can actually be valid for the
		// the parent type and just throws an exception. This function
		// just checks if 'obj` doesn’t contain an entirely invalid
		// pointer and raises a TypeError if so. Otherwise returns
		// the pointer. Use MRuby’s RDATA_PTR() directly if you don’t
		// want an exception in this case.
		template<typename T>
		T* Get_Data_Ptr(mrb_state* p_state, mrb_value obj)
		{
			T* p_result = (T*) DATA_PTR(obj);
			if (!p_result) {
				mrb_raise(p_state, MRB_TYPE_ERROR(p_state), "Unexpected NULL pointer. This is most likely an SMC bug.");
				return NULL; // Not reached
			}

			return p_result;
		}

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
