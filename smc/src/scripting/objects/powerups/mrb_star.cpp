#include "mrb_star.h"
#include "mrb_powerup.h"
#include "../../../level/level.h"
#include "../../../core/sprite_manager.h"
#include "../../../objects/star.h"

/**
 * Class: Star
 *
 * Parent: [Powerup](powerup.html)
 * {: .superclass}
 *
 * _Stars_ are the most powerful powerups in the game. Maryo gains the
 * ability to jump higher, gets resistant against all kinds of enemies,
 * etc. Stars wildly jump around and are hard to catch. Even if catched,
 * its effect only lasts a fixed period of time (which is hardcoded into
 * SMC, thus not configurable via the scripting API).
 */

using namespace SMC;
using namespace SMC::Scripting;

// Extern
struct RClass* SMC::Scripting::p_rcStar = NULL;

/**
 * Method: Star::new
 *
 *   new() → a_star
 *
 * Creates a new star powerup with the default values.
 */
static mrb_value Initialize(mrb_state* p_state, mrb_value self)
{
	cjStar* p_star = new cjStar(pActive_Level->m_sprite_manager);

	DATA_PTR(self) = p_star;
	DATA_TYPE(self) = &rtSMC_Scriptable;

	// This is a generated object
	p_star->Set_Spawned(true);

	// Let SMC manage the memory
	pActive_Level->m_sprite_manager->Add(p_star);

	return self;
}

/**
 * Method: Star#activate!
 *
 *   activate!()
 *
 * Apply the item to Maryo. Doing so will destroy the star. If
 * Maryo already has star effect, the star effect time is reset
 * to 0.
 */
static mrb_value Activate(mrb_state* p_state, mrb_value self)
{
	cjStar* p_star = Get_Data_Ptr<cjStar>(p_state, self);
	p_star->Activate();
	return mrb_nil_value();
}

/**
 * Method: Star#glimming=
 *
 *   glimming=( bool ) → bool
 *
 * TODO: Docs.
 */
static mrb_value Set_Glim_Mode(mrb_state* p_state, mrb_value self)
{
	mrb_bool glim;
	mrb_get_args(p_state, "b", &glim);

	cjStar* p_star = Get_Data_Ptr<cjStar>(p_state, self);
	p_star->m_glim_mod = glim;

	return mrb_bool_value(glim);
}

/**
 * Method: Star#glimming?
 *
 *   glimming?() → true or false
 *
 * TODO: Docs.
 */
static mrb_value Get_Glim_Mode(mrb_state* p_state, mrb_value self)
{
	cjStar* p_star = Get_Data_Ptr<cjStar>(p_state, self);
	return mrb_bool_value(p_star->m_glim_mod);
}

void SMC::Scripting::Init_Star(mrb_state* p_state)
{
	p_rcStar = mrb_define_class(p_state, "Star", p_rcPowerup);
	MRB_SET_INSTANCE_TT(p_rcStar, MRB_TT_DATA);

	mrb_define_method(p_state, p_rcStar, "initialize", Initialize, MRB_ARGS_NONE());
	mrb_define_method(p_state, p_rcStar, "activate!", Activate, MRB_ARGS_NONE());
	mrb_define_method(p_state, p_rcStar, "glimming=", Set_Glim_Mode, MRB_ARGS_REQ(1));
	mrb_define_method(p_state, p_rcStar, "glimming?", Get_Glim_Mode, MRB_ARGS_NONE());
}
