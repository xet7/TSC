#include "mrb_path.h"
#include "../sprites/mrb_sprite.h"
#include "../../../objects/sprite.h"
#include "../../../objects/path.h"
#include "../../../core/sprite_manager.h"
#include "../../../level/level.h"

/**
 * Class: Path
 * Parent: [Sprite](sprite.html)
 *
 * A _path_ is a usually invisible line objects in the level
 * (e.g. platforms) follow. While the movement could usually
 * be achieved with pure scripting using timers, paths are
 * implemented purely in C++ and as such perform better. On
 * the other hands, paths are limited to a defined number of
 * movements, while scripting your movement allows all kinds of
 * crazy things to be done.
 */

using namespace SMC;
using namespace SMC::Scripting;

// Extern
struct RClass* SMC::Scripting::p_rcPath = NULL;

/**
 * Method: Path::new
 *
 *   new() → a_path
 *
 * Creates a new path with the default values.
 */
static mrb_value Initialize(mrb_state* p_state, mrb_value self)
{
	cPath* p_path = new cPath(pActive_Level->m_sprite_manager);
	DATA_PTR(self) = p_path;
	DATA_TYPE(self) = &rtSMC_Scriptable;

	// This is a generated object
	p_path->Set_Spawned(true);

	// Let SMC manage the memory
	pActive_Level->m_sprite_manager->Add(p_path);

	return self;
}

/**
 * Method: Path#identifier=
 *
 *   identifier=( str ) → str
 *
 * Specify this path’s identifier.
 *
 * #### Parameters
 * str
 * : The path’s new identifier.
 */
static mrb_value Set_Identifier(mrb_state* p_state, mrb_value self)
{
	char* ident = NULL;
	mrb_get_args(p_state, "z", &ident);

	cPath* p_path = Get_Data_Ptr<cPath>(p_state, self);
	p_path->Set_Identifier(ident);

	return mrb_str_new_cstr(p_state, ident);
}

/**
 * Method: Path#identifier
 *
 *   identifier() → a_string or nil
 *
 * Returns the path’s current identifier as a string. If no
 * identifier has been set, returns `nil`.
 */
static mrb_value Get_Identifier(mrb_state* p_state, mrb_value self)
{
	cPath* p_path = Get_Data_Ptr<cPath>(p_state, self);

	if (p_path->m_identifier.empty())
		return mrb_nil_value();
	else
		return mrb_str_new_cstr(p_state, p_path->m_identifier.c_str());
}

/**
 * Method: Path#show_line=
 *
 *   show_line=( bool ) → bool
 *
 * Specify whether or not the path should have its moving line
 * drawn (not to draw is the default). This feature is EXPERIMENTAL
 * and usually does not work as expected.
 *
 * #### Parameters
 * bool
 * : Whether or not to draw the line.
 */
static mrb_value Set_Show_Line(mrb_state* p_state, mrb_value self)
{
	mrb_bool show;
	mrb_get_args(p_state, "b", &show);

	cPath* p_path = Get_Data_Ptr<cPath>(p_state, self);
	p_path->Set_Show_Line(show);

	return mrb_bool_value(show);
}

/**
 * Method: Path#show_line?
 *
 * Returns `true` or `false` depending on whether the path’s
 * movement line should be drawn.
 */
static mrb_value Does_Show_Line(mrb_state* p_state, mrb_value self)
{
	cPath* p_path = Get_Data_Ptr<cPath>(p_state, self);
	return mrb_bool_value(p_path->m_show_line);
}

/**
 * Method: Path#rewind=
 *
 *   rewind=( bool ) → bool
 *
 * Specify whether or not to rewind when the object moving along
 * the path reached the paths end. The default is not to rewind,
 * i.e. to mirror and move the path backwards.
 *
 * #### Parameters
 * bool
 * : Whether or not to rewind.
 */
static mrb_value Set_Rewind(mrb_state* p_state, mrb_value self)
{
	mrb_bool rewind;
	mrb_get_args(p_state, "b", &rewind);

	cPath* p_path = Get_Data_Ptr<cPath>(p_state, self);
	p_path->Set_Rewind(rewind);

	return mrb_bool_value(rewind);
}

/**
 * Method: Path#rewind?
 *
 *   rewind?() → true or false
 *
 * Returns `true` or `false` depending on whether this path
 * mirrors (`false`) or rewinds (`true`).
 */
static mrb_value Does_Rewind(mrb_state* p_state, mrb_value self)
{
	cPath* p_path = Get_Data_Ptr<cPath>(p_state, self);
	return mrb_bool_value(p_path->m_rewind);
}

void SMC::Scripting::Init_Path(mrb_state* p_state)
{
	p_rcPath = mrb_define_class(p_state, "Path", p_rcSprite);
	MRB_SET_INSTANCE_TT(p_rcPath, MRB_TT_DATA);

	mrb_define_method(p_state, p_rcPath, "initialize", Initialize, MRB_ARGS_NONE());
	mrb_define_method(p_state, p_rcPath, "identifier=", Set_Identifier, MRB_ARGS_REQ(1));
	mrb_define_method(p_state, p_rcPath, "identifier", Get_Identifier, MRB_ARGS_NONE());
	mrb_define_method(p_state, p_rcPath, "show_line=", Set_Show_Line, MRB_ARGS_REQ(1));
	mrb_define_method(p_state, p_rcPath, "show_line?", Does_Show_Line, MRB_ARGS_NONE());
	mrb_define_method(p_state, p_rcPath, "rewind=", Set_Rewind, MRB_ARGS_REQ(1));
	mrb_define_method(p_state, p_rcPath, "rewind?", Does_Rewind, MRB_ARGS_NONE());
}
