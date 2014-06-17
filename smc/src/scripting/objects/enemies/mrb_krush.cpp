#include "../../../enemies/krush.hpp"
#include "../../../level/level.hpp"
#include "../../../core/sprite_manager.hpp"
#include "../../../core/property_helper.hpp"
#include "mrb_enemy.hpp"
#include "mrb_krush.hpp"

/**
 * Class: Krush
 *
 * Parent: [Enemy](enemy.html)
 * {: .superclass}
 *
 * _Krush_! This big dinosour-like enemy may just do with you what its
 * name says. It even requires _two_ hits to be defeated!
 */

using namespace SMC;
using namespace SMC::Scripting;

// Extern
struct RClass* SMC::Scripting::p_rcKrush = NULL;

/**
 * Method: Krush::new
 *
 *   new() → a_krush
 *
 * Creates a new krush with the default values.
 */
static mrb_value Initialize(mrb_state* p_state,  mrb_value self)
{
	cKrush* p_krush = new cKrush(pActive_Level->m_sprite_manager);
	DATA_PTR(self) = p_krush;
	DATA_TYPE(self) = &rtSMC_Scriptable;

	// This is a generated object
	p_krush->Set_Spawned(true);

	// Let SMC manage the memory
	pActive_Level->m_sprite_manager->Add(p_krush);

	return self;
}

void SMC::Scripting::Init_Krush(mrb_state* p_state)
{
	struct RClass* p_rcKrush = mrb_define_class(p_state, "Krush", mrb_class_get(p_state, "Enemy"));
	MRB_SET_INSTANCE_TT(p_rcKrush, MRB_TT_DATA);

	mrb_define_method(p_state, p_rcKrush, "initialize", Initialize, MRB_ARGS_NONE());
}
