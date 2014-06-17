#include "../../../objects/lava.hpp"
#include "../../../core/sprite_manager.hpp"
#include "../../../level/level.hpp"
#include "mrb_lava.hpp"

/**
 * Class: Lava
 *
 * Parent: [AnimatedSprite](animatedsprite.html)
 * {: .superclass}
 *
 * _Lava_ makes any enemy and Maryo immediately and instantly die.
 */

using namespace SMC;
using namespace SMC::Scripting;


/**
 * Method: Lava::new
 *
 *   new() → a_lava
 *
 * Creates a new piece of lava.
 *
 * ##### Return value
 * The newly created instance.
 */
static mrb_value Initialize(mrb_state* p_state, mrb_value self)
{
	cLava* p_lava = new cLava(pActive_Level->m_sprite_manager);
	DATA_PTR(self) = p_lava;
	DATA_TYPE(self) = &rtSMC_Scriptable;

	// This is a generated object
	p_lava->Set_Spawned(true);

	// Let SMC manage the memory
	pActive_Level->m_sprite_manager->Add(p_lava);

	return self;
}

void SMC::Scripting::Init_Lava(mrb_state* p_state)
{
	struct RClass* p_rcLava = mrb_define_class(p_state, "Lava", mrb_class_get(p_state, "AnimatedSprite"));
	MRB_SET_INSTANCE_TT(p_rcLava, MRB_TT_DATA);

	mrb_define_method(p_state, p_rcLava, "initialize", Initialize, MRB_ARGS_NONE());
}
