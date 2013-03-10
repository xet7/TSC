// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../scripting.h"
#include "mrb_moving_sprite.h"
#include "mrb_animated_sprite.h"

using namespace SMC;
using namespace SMC::Scripting;

// Extern
struct RClass* SMC::Scripting::p_rcAnimated_Sprite = NULL;
struct mrb_data_type SMC::Scripting::rtAnimated_Sprite = {"AnimatedSprite", NULL};

void SMC::Scripting::Init_Animated_Sprite(mrb_state* p_state)
{
	p_rcAnimated_Sprite = mrb_define_class(p_state, "AnimatedSprite", p_rcMoving_Sprite);
}
