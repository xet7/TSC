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

#include "../core/global_game.h"

// Some defines to ease use of mruby
#define MRB_ARGUMENT_ERROR(mrb) (mrb_class_obj_get(mrb, "ArgumentError"))

namespace SMC {
	namespace Scripting {

		// This table maps C++ class names (type_info.name()) to Ruby class names.
		typedef std::map<std::string, std::string> ClassMap;
		// Name of the global hash mapping the UIDs to
		// objects of Sprite and its subclasses.
		const std::string UID_TABLE_NAME = "UIDS";

		// Maps C++ class names to MRuby class names
		extern ClassMap type2class;
		// Maps Ruby class names to C++ class names.
		extern ClassMap class2type;

		void Initialize_Scripting();

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
			mrb_value Wrap_MRuby_Object_Around_Sprite(cSprite* p_sprite);
		};
	};
};

#endif
