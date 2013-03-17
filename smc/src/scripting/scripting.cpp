// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "scripting.h"
#include "../level/level.h"
#include "../level/level_player.h"
#include "../core/sprite_manager.h"
#include "../core/property_helper.h"

#include "objects/mrb_smc.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

/*****************************************************************************
 Scripting organisation

Yeah, scripting! SMC embeds mruby (https://github.com/mruby/mruby), a minimal
Ruby implementation for boosting your level design. The cMRubyInterpreter class
wraps the mruby_state struct in a more convenient way and also initialises it for
use with SMC. Each time a level is loaded, a new clean instance of this class
is created (and of course destroyed on level ending).

After the setup is done, the constructor calls the Load_Scripts()
private member which in turn feeds the "main.rb" file in the scripting/ directory
of your SMC installation into the mruby interpreter. This script, which may
be added by users if they want to provide additional functionality globally,
has access to a very minimalistic version of #require, which just allows for
loading scripts relative to the scripting/ directory and also has some quirks,
such as the inability to define toplevel constants without an explicit :: at
the beginning, e.g.

  module Foo
  end

will not work, but

  module ::Foo
  end

will. The default main.rb file only contains the bare minimum needed to
correctly initialise the scripting functionality and users are advised to
not remove this code. Removing e.g. the Eventable module will have very bad
effects, but on the other hand this gives you the full power of Ruby to
hook in everything you want to. Finally, the main.rb script is required to
call the SMC::setup method, which loads all the C++ wrapper classes (i.e.
Sprite, LevelPlayer, etc.) into the interpreter.

*****************************************************************************/

namespace SMC
{

	namespace Scripting
	{

		// The path to the directory containing the global scripts.
		boost::filesystem::path scripting_dir = SMC::utf8_to_path(DATA_DIR) / "scripting";

		cMRuby_Interpreter::cMRuby_Interpreter(cLevel* p_level)
		{
			// Set member variables
			mp_level = p_level;
			mp_mruby = mrb_open();

			// Load our extensions into mruby
			Load_Scripts();
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
				errormsg = format_mruby_error(mp_mruby, mp_mruby->exc);
				return false;
			}
			else
				return true;
		}

		void cMRuby_Interpreter::Load_Scripts()
		{
			// Create the SMC module
			Init_SMC(mp_mruby);

			// Load the main scripting file. This file is required to call
			// SMC::setup, which loads all of the wrapper classes into
			// the interpreter.
			boost::filesystem::path mainfile = scripting_dir / "main.rb";
			boost::filesystem::ifstream file(mainfile);
			if (!file.is_open()) {
				std::cerr << "Failed to open main scripting file '" << mainfile.generic_string().c_str() << "' for reading." << std::endl;
				std::cerr << "Scripting will not work correctly." << std::endl;
				return;
			}
			else {
				mrb_load_string(mp_mruby, readfile(file).c_str());
				if (mp_mruby->exc){
					std::cerr << "Warning: Error loading main script: ";
					std::cerr << format_mruby_error(mp_mruby, mp_mruby->exc);
					std::cerr << std::endl;
				}
				file.close();
			}


		}

	};

};
