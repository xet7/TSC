#include "../../../objects/goldpiece.hpp"
#include "../../../core/sprite_manager.hpp"
#include "../../../level/level.hpp"
#include "mrb_jumping_goldpiece.hpp"
#include "mrb_goldpiece.hpp"

using namespace SMC;
using namespace SMC::Scripting;

/**
 * Class: FallingGoldpiece
 *
 * Parent: [Goldpiece](goldpiece.html)
 * {: .superclass}
 *
 * This kind of goldpiece jumps around when it falls down
 * something, i.e. it is subject to gravity and moves around.
 * This is goldpiece type used when you eliminate an enemy
 * by means of a fireball.
 */


/**
 * Method: FallingGoldpiece::new
 *
 *   new() → a_falling_goldpiece
 *
 * Creates a new instance of this class.
 */
static mrb_value Initialize(mrb_state* p_state, mrb_value self)
{
    cJGoldpiece* p_fgp = new cJGoldpiece(pActive_Level->m_sprite_manager);
    DATA_PTR(self) = p_fgp;
    DATA_TYPE(self) = &rtSMC_Scriptable;

    p_fgp->Set_Spawned(true);
    pActive_Level->m_sprite_manager->Add(p_fgp);

    return self;
}

void SMC::Scripting::Init_FallingGoldpiece(mrb_state* p_state)
{
    struct RClass* p_rcFalling_Goldpiece = mrb_define_class(p_state, "FallingGoldpiece", mrb_class_get(p_state, "Goldpiece"));
    MRB_SET_INSTANCE_TT(p_rcFalling_Goldpiece, MRB_TT_DATA);

    mrb_define_method(p_state, p_rcFalling_Goldpiece, "initialize", Initialize, MRB_ARGS_NONE());
}
