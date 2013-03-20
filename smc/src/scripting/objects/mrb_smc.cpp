// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "mrb_smc.h"
#include "../../core/property_helper.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

/**
 * Module: SMC
 *
 * Module encapsulating stuff related to the game itself.
 */

using namespace SMC;

// Extern
struct RClass* SMC::Scripting::p_rmSMC = NULL;

/**
 * Method: SMC::require
 *
 *   require( path )
 *
 * Minimalistic file loading capability. Loads a file
 * relative to SMC’s scripting/ directory into the running
 * MRuby instance.
 *
 * Using this outside of the initialisation sequence doesn’t
 * make much sense.
 */
static mrb_value Require(mrb_state* p_state, mrb_value self)
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
	boost::filesystem::path scriptfile = Scripting::scripting_dir / path;
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

/**
 * Method: SMC::setup
 *
 *   setup()
 *
 * Main setup method. This method *must* be called during the
 * initialisation sequence in `main.rb`, otherwise scripting will badly
 * malfunction.
 */
static mrb_value Setup(mrb_state* p_state, mrb_value self)
{
	// Note that pActive_Level is not yet set when this
	// method is called.

	SMC::Scripting::Load_Wrappers(p_state);
	return mrb_nil_value();
}

void SMC::Scripting::Init_SMC(mrb_state* p_state)
{
	p_rmSMC = mrb_define_module(p_state, "SMC");

	mrb_define_module_function(p_state, p_rmSMC, "require", Require, ARGS_REQ(1));
	mrb_define_module_function(p_state, p_rmSMC, "setup", Setup, ARGS_NONE());
}
