// -*- c++ -*-
#ifndef SMC_SCRIPTING_HPP
#define SMC_SCRIPTING_HPP
#include "../core/global_basic.h"
#include "../core/global_game.h"
#include "objects/mrb_smc.h"

// Some defines to ease use of mruby
#define MRB_ARGUMENT_ERROR(mrb) (mrb_class_get(mrb, "ArgumentError"))
#define MRB_RUNTIME_ERROR(mrb) (mrb_class_get(mrb, "RuntimeError"))
#define MRB_TYPE_ERROR(mrb) (mrb_class_get(mrb, "TypeError"))
#define MRB_NOTIMP_ERROR(mrb) (mrb_class_get(mrb, "NotImplementedError"))
#define MRB_RANGE_ERROR(mrb) (mrb_class_get(mrb, "RangeError"))

namespace SMC {
	namespace Scripting {

		// We don’t use mruby’s C typechecks, but mruby wants
		// an mrb_data_type nevertheless from us. So we set
		// it for all our objects to this one.
		extern struct mrb_data_type rtSMC_Scriptable;

		// Load all MRuby wrapper classes for the C++ classes
		// into the given mruby state. Called by SMC::setup
		// in mruby land.
		void Load_Wrappers(mrb_state* p_state);
		// Takes a C(++) string and directly returns an MRuby
		// symbol object (not an mrb_sym!) for it.
		inline mrb_value str2sym(mrb_state* mrb, std::string str){ return mrb_symbol_value(mrb_intern_cstr(mrb, str.c_str())); }

		/**
		 * Shorthand for doing
		 *   DATA_GET_PTR(p_state, obj, &rtSMC_Scriptable)
		 * over and over with a security NULL check.
		 */
		template<typename T>
		T* Get_Data_Ptr(mrb_state* p_state, mrb_value obj)
		{
			T* p_result = static_cast<T*>(mrb_data_get_ptr(p_state, obj, &rtSMC_Scriptable));
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
			// (including syntax errors), false is returned,
			// true otherwise. `contextname' is purely informational
			// and only ever used in exception messages.
			// This method prints exceptions to standard error.
			bool Run_Code(const std::string& code, const std::string& contextname);
			// Execute MRuby code found in a file, using the filename
			// as the context name. Otherwise has the same
			// semantics as Run_Code().
			bool Run_File(const boost::filesystem::path& filepath);
			// Execute MRuby code in the given parsing context.
			// This method only does raw code execution, no
			// exception inspection is done for you. It’s basically
			// a wrapper around mrb_load_nstring_cxt().
			mrb_value Run_Code_In_Context(const std::string& code, mrbc_context* p_context);
			// Registers an MRuby callback to be called on the next
			// call to Evaluate_Timer_Callbacks(). `callback'
			// is an MRuby proc.
			// This method is threadsafe.
			void Register_Callback(mrb_value callback);
			// Runs all callbacks whose timers have fired.
			// This method is threadsafe.
			void Evaluate_Timer_Callbacks();
			// Returns the underlying mrb_state*.
			mrb_state* Get_MRuby_State();
			// Returns the cLevel* we’re associated with.
			cLevel* Get_Level();
		private:
			mrb_state* mp_mruby;
			cLevel* mp_level;
			std::vector<mrb_value> m_callbacks;
			boost::mutex m_callback_mutex;

			// Does basic setup and then executes the main.rb file.
			void Load_Scripts();
		};
	};
};

#endif
