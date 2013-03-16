// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "scripting.h"
#include "../level/level.h"
#include "../level/level_player.h"
#include "../core/sprite_manager.h"
#include "../core/property_helper.h"

#include "objects/mrb_sprite.h"
#include "objects/mrb_moving_sprite.h"
#include "objects/mrb_animated_sprite.h"
#include "objects/mrb_level_player.h"
#include "objects/mrb_uids.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

// The path to the directory containing the global scripts.
static boost::filesystem::path s_scripting_dir(SMC::utf8_to_path(DATA_DIR) / "scripting");

/**
 * Reads in `file' completely. Does not call close().
 */
static std::string readfile(boost::filesystem::ifstream& file)
{
	std::string content;
	std::string line;
	while(!file.eof()) {
		std::getline(file, line);
		content.append(line);
		content.append("\n");
	}

	return content;
}

/**
 * The #require method for scripting.
 */
static mrb_value smc_require(mrb_state* p_state, mrb_value self)
{
	using namespace SMC;

	// Get the path argument
	char* cpath = NULL;
	mrb_get_args(p_state, "z", &cpath);

	// Append ".rb" and convert to a platform-independent boost path
	std::string spath(cpath);
	spath.append(".rb");
	boost::filesystem::path path = utf8_to_path(spath);

	// Disallow absolute pathes, we don’t want load external files
	// accidentally
	if (path.is_absolute())
		mrb_raise(p_state, MRB_ARGUMENT_ERROR(p_state), "Absolute paths are not allowed.");

	// Open the MRuby file for reading
	boost::filesystem::path scriptfile = s_scripting_dir / path;
	boost::filesystem::ifstream file(scriptfile);
	debug_print("require: Loading '%s'\n", scriptfile.generic_string().c_str());
	if (!file.is_open())
		mrb_raisef(p_state, MRB_RUNTIME_ERROR(p_state), "Cannot open file '%s' for reading", scriptfile.generic_string().c_str());

	// Compile and run the MRuby code
	mrb_load_string(p_state, readfile(file).c_str());

	// Cleanup
	file.close();
	if (p_state->exc)
		mrb_exc_raise(p_state, mrb_obj_value(p_state->exc));

	// Finish
	return mrb_nil_value();
}

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
			// Define the #require method
			mrb_define_method(mp_mruby, mp_mruby->kernel_module, "require", smc_require, ARGS_REQ(1));

			// Load the main scripting file
			boost::filesystem::path mainfile = s_scripting_dir / "main.mrb";
			boost::filesystem::ifstream file(mainfile);
			if (!file.is_open()) {
				std::cerr << "Failed to open main scripting file '" << mainfile.generic_string().c_str() << "' for reading." << std::endl;
				std::cerr << "Scripting will not be available." << std::endl;
				return;
			}
			else {
				mrb_load_string(mp_mruby, readfile(file).c_str());
				if (mp_mruby->exc){
					std::cerr << "Warning: Error loading main script: ";
					std::cerr << mrb_string_value_ptr(mp_mruby, mrb_funcall(mp_mruby, mrb_obj_value(mp_mruby->exc), "message", 0));
					std::cerr << std::endl;
				}
				file.close();
			}

			Init_Sprite(mp_mruby);
			Init_Moving_Sprite(mp_mruby);
			Init_Animated_Sprite(mp_mruby);
			Init_Level_Player(mp_mruby);
			Init_UIDS(mp_mruby); // Call this last so it can rely on the other MRuby classes to be defined
		}

	};

};
