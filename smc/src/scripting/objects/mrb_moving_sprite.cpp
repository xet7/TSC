// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../scripting.h"
#include "mrb_sprite.h"
#include "mrb_moving_sprite.h"

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

void SMC::Scripting::Init_Moving_Sprite(mrb_state* p_state)
{
	p_rcMoving_Sprite = mrb_define_class(p_state, "MovingSprite", p_rcSprite);
}
