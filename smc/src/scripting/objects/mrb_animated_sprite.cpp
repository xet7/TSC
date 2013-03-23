// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../scripting.h"
#include "mrb_moving_sprite.h"
#include "mrb_animated_sprite.h"

/**
 * Class: AnimatedSprite
 *
 * Parent: [MovingSprite](movingsprite.html)
 * {: .superclass}
 *
 * Currently there are no objects exposed to the MRuby API
 * that are _AnimatedSprites_ without being an instance
 * of [MovingSprite](movingsprite.html).
 */

using namespace SMC;
using namespace SMC::Scripting;

// Extern
struct RClass* SMC::Scripting::p_rcAnimated_Sprite = NULL;
struct mrb_data_type SMC::Scripting::rtAnimated_Sprite = {"AnimatedSprite", NULL};

static mrb_value Initialize(mrb_state* p_state,  mrb_value self)
{
	mrb_raise(p_state, MRB_NOTIMP_ERROR(p_state), "Cannot create instances of this class.");
	return self; // Not reached
}

void SMC::Scripting::Init_Animated_Sprite(mrb_state* p_state)
{
	p_rcAnimated_Sprite = mrb_define_class(p_state, "AnimatedSprite", p_rcMoving_Sprite);

	mrb_define_method(p_state, p_rcAnimated_Sprite, "initialize", Initialize, ARGS_NONE());
}
