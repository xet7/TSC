#include "mrb_powerup.h"
#include "../sprites/mrb_animated_sprite.h"
#include "../../../objects/powerup.h"

/**
 * Class: Powerup
 *
 * Parent: [AnimatedSprite](animatedsprite.html)
 * {: .superclass}
 *
 * _Powerups_ are goodies (well, or badies) that affect Maryo’s
 * state somehow. This class is the baseclass for all powerups.
 */

using namespace SMC;
using namespace SMC::Scripting;

// Extern
struct RClass* SMC::Scripting::p_rcPowerup = NULL;

static mrb_value Initialize(mrb_state* p_state, mrb_value self)
{
	mrb_raise(p_state, MRB_NOTIMP_ERROR(p_state), "Cannot create instances of this class.");
	return self; // Not reached
}

void SMC::Scripting::Init_Powerup(mrb_state* p_state)
{
	p_rcPowerup = mrb_define_class(p_state, "Powerup", p_rcAnimated_Sprite);
	MRB_SET_INSTANCE_TT(p_rcPowerup, MRB_TT_DATA);

	mrb_define_method(p_state, p_rcPowerup, "initialize", Initialize, MRB_ARGS_NONE());
}
