#include "mrb_spinbox.h"
#include "../../../objects/spinbox.h"
#include "../../../level/level.h"
#include "../../../core/sprite_manager.h"
#include "mrb_box.h"

/**
 * Class SpinBox:
 *
 * Parent [Box](box.html)
 * {: .superclass}
 *
 * The _Spin Box_ is an empty box that gets passable
 * for a short time if jumped against it from below
 * (or if a shell is thrown against it from the side).
 */

using namespace SMC;
using namespace SMC::Scripting;

// Extern
struct RClass* SMC::Scripting::p_rcSpin_Box = NULL;

/**
 * Method: SpinBox::new
 *
 * Creates a new SpinBox with the default values.
 */
static mrb_value Initialize(mrb_state* p_state, mrb_value self)
{
	cSpinBox* p_box = new cSpinBox(pActive_Level->m_sprite_manager);
	DATA_PTR(self) = p_box;
	DATA_TYPE(self) = &rtSMC_Scriptable;

	// This is a generated object
	p_box->Set_Spawned(true);

	// Let SMC manage the memory
	pActive_Level->m_sprite_manager->Add(p_box);

	return self;
}

/**
 * Method: SpinBox#stop
 *
 *   stop()
 *
 * Stop the spinning started by Box#activate.
 */
static mrb_value Stop(mrb_state* p_state, mrb_value self)
{
	cSpinBox* p_box = Get_Data_Ptr<cSpinBox>(p_state, self);
	p_box->Stop();
	return mrb_nil_value();
}

/**
 * Method: SpinBox#spinning?
 *
 *   spinning?() → true or false
 *
 * Checks whether the box is spinning currently, and if so,
 * returns `true`, otherwise returns `false`.
 */
static mrb_value Is_Spinning(mrb_state* p_state, mrb_value self)
{
	cSpinBox* p_box = Get_Data_Ptr<cSpinBox>(p_state, self);
	return mrb_bool_value(p_box->m_spin);
}

void SMC::Scripting::Init_SpinBox(mrb_state* p_state)
{
	p_rcSpin_Box = mrb_define_class(p_state, "SpinBox", p_rcBox);
	MRB_SET_INSTANCE_TT(p_rcSpin_Box, MRB_TT_DATA);

	mrb_define_method(p_state, p_rcSpin_Box, "initialize", Initialize, MRB_ARGS_NONE());
	mrb_define_method(p_state, p_rcSpin_Box, "stop", Stop, MRB_ARGS_NONE());
	mrb_define_method(p_state, p_rcSpin_Box, "spinning?", Is_Spinning, MRB_ARGS_NONE());
}
