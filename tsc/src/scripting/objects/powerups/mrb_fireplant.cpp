#include "mrb_fireplant.hpp"
#include "mrb_powerup.hpp"
#include "../../../level/level.hpp"
#include "../../../core/sprite_manager.hpp"
#include "../../../objects/powerup.hpp"

/**
 * Class: Fireplant
 *
 * Parent: [Powerup](powerup.html)
 * {: .superclass}
 *
 * _Fireplants_ are powerups that grant Maryo the skill to throw
 * fireballs agains enemies. Fireplants don’t move by default.
 */

using namespace TSC;
using namespace TSC::Scripting;


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
    DATA_TYPE(self) = &rtTSC_Scriptable;

    // This is a generated object
    p_fireplant->Set_Spawned(true);

    // Let TSC manage the memory
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

void TSC::Scripting::Init_Fireplant(mrb_state* p_state)
{
    struct RClass* p_rcFireplant = mrb_define_class(p_state, "Fireplant", mrb_class_get(p_state, "Powerup"));
    MRB_SET_INSTANCE_TT(p_rcFireplant, MRB_TT_DATA);

    mrb_define_method(p_state, p_rcFireplant, "initialize", Initialize, MRB_ARGS_NONE());
    mrb_define_method(p_state, p_rcFireplant, "activate!", Activate, MRB_ARGS_NONE());
}
