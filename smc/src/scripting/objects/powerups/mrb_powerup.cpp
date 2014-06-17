#include "mrb_powerup.hpp"
#include "../sprites/mrb_animated_sprite.hpp"
#include "../../../objects/powerup.hpp"
#include "../../events/event.hpp"

/**
 * Class: Powerup
 *
 * Parent: [AnimatedSprite](animatedsprite.html)
 * {: .superclass}
 *
 * _Powerups_ are goodies (well, or badies) that affect Maryo’s
 * state somehow. This class is the baseclass for all powerups.
 *
 * Events
 * ------
 *
 * Activate
 * : This event is triggered when the powerup is applied to
 *   the player. Do not apply other powerups to the player
 *   inside the event handler for this, this will cause
 *   undefined behaviour.
 */

using namespace SMC;
using namespace SMC::Scripting;

MRUBY_IMPLEMENT_EVENT(activate);

// Extern
struct RClass* SMC::Scripting::p_rcPowerup = NULL;

static mrb_value Initialize(mrb_state* p_state, mrb_value self)
{
	mrb_raise(p_state, MRB_NOTIMP_ERROR(p_state), "Cannot create instances of this class.");
	return self; // Not reached
}

/**
 * Method: Powerup#activate!
 *
 *   activate!()
 *
 * Apply the powerup to the player. This method is not really
 * useful on itself, it is overridden in the subclasses to
 * do something meaningful. Currently, it just issues
 * the _Activate_ event.
 */
static mrb_value Activate(mrb_state* p_state, mrb_value self)
{
	cPowerUp* p_powerup = Get_Data_Ptr<cPowerUp>(p_state, self);
	p_powerup->Activate();
	return mrb_nil_value();
}

void SMC::Scripting::Init_Powerup(mrb_state* p_state)
{
	struct RClass* p_rcPowerup = mrb_define_class(p_state, "Powerup", p_rcAnimated_Sprite);
	MRB_SET_INSTANCE_TT(p_rcPowerup, MRB_TT_DATA);

	mrb_define_method(p_state, p_rcPowerup, "initialize", Initialize, MRB_ARGS_NONE());
	mrb_define_method(p_state, p_rcPowerup, "activate!", Activate, MRB_ARGS_NONE());

	mrb_define_method(p_state, p_rcPowerup, "on_activate", MRUBY_EVENT_HANDLER(activate), MRB_ARGS_BLOCK());
}
