#include "mrb_smc.h"
#include "../../core/game_core.h"
#include "../../core/property_helper.h"
#include "../../core/filesystem/resource_manager.h"
#include "../../core/framerate.h"

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
	boost::filesystem::path scriptfile = pResource_Manager->Get_Game_Scripting_Directory() / path;
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

/**
 * Method: SMC::platform
 *
 *   platform() → a_string
 *
 * Information about the platform SMC was compiled for.
 *
 * #### Return value
 * One of `:win32`, `:linux`, `:apple`, or `:other`. Please
 * notify me if you get `:other` from this method.
 */
static mrb_value Platform(mrb_state* p_state, mrb_value self)
{
#ifdef _WIN32
	return SMC::Scripting::str2sym(p_state, "win32");
#elif __linux
	return SMC::Scripting::str2sym(p_state, "linux");
#elif __APPLE__
	return SMC::Scripting::str2sym(p_state, "apple");
#else
	return SMC::Scripting::str2sym(p_state, "other");
#endif
}

/**
 * Method: SMC::quit
 *
 *   quit()
 *
 * Instructs SMC to terminate the main loop, exiting the
 * game as if regularily closed.
 */
static mrb_value Quit(mrb_state* p_state, mrb_value self)
{
	game_exit = true;
	return mrb_nil_value();
}

/**
 * Method: SMC::exit
 *
 *   exit( exitcode )
 *
 * Immediately and forcibly terminates SMC without any
 * cleanup.
 *
 * `0 <= exitcode < 255` is the range for `exitcode`.
 */
static mrb_value Exit(mrb_state* p_state, mrb_value self)
{
	mrb_int exitcode;
	mrb_get_args(p_state, "i", &exitcode);

	exit(exitcode);
	return mrb_nil_value(); // Not reached
}

/**
 * Method: SMC::current_framerate
 *
 *   current_framerate() → integer
 *
 * The current frames per second (FPS).
 */
static mrb_value Current_Framerate(mrb_state* p_state, mrb_value self)
{
	return mrb_fixnum_value(pFramerate->m_fps);
}

/**
 * Method: SMC::average_framerate
 *
 *   average_framerate() → integer
 *
 * The average frames per second (FPS).
 */
static mrb_value Average_Framerate(mrb_state* p_state, mrb_value self)
{
	return mrb_fixnum_value(pFramerate->m_fps_average);
}

/**
 * Method: SMC::best_framerate
 *
 *   best_framerate() → integer
 *
 * The best frames per second (FPS).
 */
static mrb_value Best_Framerate(mrb_state* p_state, mrb_value self)
{
	return mrb_fixnum_value(pFramerate->m_fps_best);
}

/**
 * Method: SMC::worst_framerate
 *
 *   worst_framerate() → integer
 *
 * The worst frames per second (FPS).
 */
static mrb_value Worst_Framerate(mrb_state* p_state, mrb_value self)
{
	return mrb_fixnum_value(pFramerate->m_fps_worst);
}

/**
 * Method: SMC::version
 *
 *   version() → a_string
 *
 * SMC’s version number in the form `major.minor.patch`.
 */
static mrb_value Version(mrb_state* p_state, mrb_value self)
{
	std::stringstream ss;

	ss << SMC_VERSION_MAJOR << "." << SMC_VERSION_MINOR << "." << SMC_VERSION_PATCH;

	return mrb_str_new_cstr(p_state, ss.str().c_str());
}

void SMC::Scripting::Init_SMC(mrb_state* p_state)
{
	p_rmSMC = mrb_define_module(p_state, "SMC");

	mrb_define_module_function(p_state, p_rmSMC, "require", Require, MRB_ARGS_REQ(1));
	mrb_define_module_function(p_state, p_rmSMC, "setup", Setup, MRB_ARGS_NONE());
	mrb_define_module_function(p_state, p_rmSMC, "platform", Platform, MRB_ARGS_NONE());
	mrb_define_module_function(p_state, p_rmSMC, "quit", Quit, MRB_ARGS_NONE());
	mrb_define_module_function(p_state, p_rmSMC, "exit", Exit, MRB_ARGS_REQ(1));
	mrb_define_module_function(p_state, p_rmSMC, "current_framerate", Current_Framerate, MRB_ARGS_NONE());
	mrb_define_module_function(p_state, p_rmSMC, "average_framerate", Average_Framerate, MRB_ARGS_NONE());
	mrb_define_module_function(p_state, p_rmSMC, "best_framerate", Best_Framerate, MRB_ARGS_NONE());
	mrb_define_module_function(p_state, p_rmSMC, "worst_framerate", Worst_Framerate, MRB_ARGS_NONE());
	mrb_define_module_function(p_state, p_rmSMC, "version", Version, MRB_ARGS_NONE());
}
