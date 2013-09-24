#include "mrb_fireplant.h"
#include "mrb_powerup.h"
#include "../../../level/level.h"
#include "../../../core/sprite_manager.h"
#include "../../../objects/powerup.h"

/**
 * Class: Fireplant
 *
 * Parent: [Powerup](powerup.html)
 * {: .superclass}
 *
 * _Fireplants_ are powerups that grant Maryo the skill to throw
 * fireballs agains enemies. Fireplants don’t move by default.
 */

using namespace SMC;
using namespace SMC::Scripting;

// Extern
struct RClass* SMC::Scripting::p_rcFireplant = NULL;

/**
 * Method: Fireplant::new
 *
 *   new() → a_fireplant
 *
 * Creates a new fireplant powerup with the default values.
 */
static mrb_value Initialize(mrb_state* p_state, mrb_value self)
{
	cFirePlant* p_fireplant = new cFirePlant(pActive_Level->m_sprite_manager);

	DATA_PTR(self) = p_fireplant;
	DATA_TYPE(self) = &rtSMC_Scriptable;

	// This is a generated object
	p_fireplant->Set_Spawned(true);

	// Let SMC manage the memory
	pActive_Level->m_sprite_manager->Add(p_fireplant);

	return self;
}

/**
 * Method: Fireplant#activate!
 *
 *   activate!()
 *
 * Apply the item to Maryo. Doing so will destroy the fireplant in
 * any case, but note that applying it does not necessarily cause
 * it to have an effect on Maryo (use `LevelPlayer#type=` for
 * that). Instead, it may be stored in the item box, or even
 * just have no effect if that is already full.
 */
static mrb_value Activate(mrb_state* p_state, mrb_value self)
{
	cFirePlant* p_fireplant = Get_Data_Ptr<cFirePlant>(p_state, self);
	p_fireplant->Activate();
	return mrb_nil_value();
}

void SMC::Scripting::Init_Fireplant(mrb_state* p_state)
{
	p_rcFireplant = mrb_define_class(p_state, "Fireplant", p_rcPowerup);
	MRB_SET_INSTANCE_TT(p_rcFireplant, MRB_TT_DATA);

	mrb_define_method(p_state, p_rcFireplant, "initialize", Initialize, MRB_ARGS_NONE());
	mrb_define_method(p_state, p_rcFireplant, "activate!", Activate, MRB_ARGS_NONE());
}
