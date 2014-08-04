#ifndef SMC_SCRIPTING_EVENT_HPP
#define SMC_SCRIPTING_EVENT_HPP
#include "../scripting.hpp"
#include "../../scripting/scriptable_object.hpp"

// Defines an event handler function that forwards to the Eventable#bind
// method, passing `evtname' as the first argument. Effectively implements
// the C++ side of the "on_*" methods. The MRUBY_EVENT_HANDLER macro returns
// the name of the function defined by this macro, so you can pass the function
// to mrb_define_method.
#define MRUBY_IMPLEMENT_EVENT(evtname) \
	static mrb_value Scripting_Event_On_##evtname(mrb_state* p_state, mrb_value self) \
	{ \
	mrb_value callback; \
	mrb_get_args(p_state, "&", &callback); \
	\
	mrb_value arg = mrb_str_new_cstr(p_state, #evtname); \
	return mrb_funcall_with_block(p_state, \
		self, \
		mrb_intern_cstr(p_state, "bind"), \
		1, \
		&arg, \
		callback); \
	}
// ↑ Note mrb_funcall_with_block() takes a C array of mrb_values ↑

// Expands to the name of the function definend
// by MRUBY_IMPLEMENT_EVENT.
#define MRUBY_EVENT_HANDLER(evtname) Scripting_Event_On_##evtname

namespace SMC {
	namespace Scripting {
		class cEvent
		{
		public:
			void Fire(cMRuby_Interpreter* p_mruby, Scripting::cScriptable_Object* p_obj);
			virtual string Event_Name();
		protected:
			virtual void Run_MRuby_Callback(cMRuby_Interpreter* p_mruby, mrb_value callback);
		};
	};
};
#endif
