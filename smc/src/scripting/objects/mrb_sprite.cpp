// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../scripting.h"
#include "mrb_sprite.h"

using namespace SMC;

void Scripting::Init_Sprite(mrb_state* p_state)
{
	mrb_define_class(p_state, "Sprite", p_state->object_class);
}
