// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../scripting.h"
#include "mrb_sprite.h"
#include "mrb_moving_sprite.h"
#include "../../objects/movingsprite.h"

/**
 * Class: MovingSprite
 *
 * Parent: [Sprite](sprite.html)
 * {: .superclass}
 *
 * Everything that is moving on the screen is considered by SMC to be a
 * _MovingSprite_. It is the superclass of most on-screen objects and as
 * such the methods defined here are available to most other objects,
 * e.g. the [Player](player.html) or [enemies](enemy.html).
 *
 * This class may not be instanciated directly.
 */

using namespace SMC;
using namespace SMC::Scripting;

// Extern
struct RClass* SMC::Scripting::p_rcMoving_Sprite = NULL;
struct mrb_data_type SMC::Scripting::rtMoving_Sprite = {"MovingSprite", NULL};

/**
 * Method: MovingSprite#accelerate
 *
 *   accelerate( xadd, yadd )
 *
 * Add to both the horizontal and the vertical velocity at once.
 *
 * #### Parameters
 * xadd
 * : What to add to the horizontal velocity. May include fractions.
 *
 * yadd
 * : What to add to the vertical velocity. May include fractions.
 */
static mrb_value Accelerate(mrb_state* p_state,  mrb_value self)
{
	cMovingSprite* p_sprite = Get_Data_Ptr<cMovingSprite>(p_state, self);
	mrb_float velx;
	mrb_float vely;
	mrb_get_args(p_state, "ff", &velx, &vely);

	p_sprite->Add_Velocity(velx, vely);
}


void SMC::Scripting::Init_Moving_Sprite(mrb_state* p_state)
{
	p_rcMoving_Sprite = mrb_define_class(p_state, "MovingSprite", p_rcSprite);

	mrb_define_method(p_state, p_rcMoving_Sprite, "accelerate!", Accelerate, ARGS_REQ(2));
}
