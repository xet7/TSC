// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "mrb_enemy.h"
#include "mrb_animated_sprite.h"
#include "../../enemies/enemy.h"

using namespace SMC;
using namespace SMC::Scripting;

// Extern
struct RClass* SMC::Scripting::p_rcEnemy     = NULL;
struct mrb_data_type SMC::Scripting::rtEnemy = {"Enemy", NULL};

/**
 * Method: Enemy#kill!
 *
 *   kill!()
 *
 * TODO: Docs.
 */
static mrb_value Kill(mrb_state* p_state,  mrb_value self)
{
	cEnemy* p_enemy = NULL;
	Data_Get_Struct(p_state, self, &rtEnemy, p_enemy);

	p_enemy->Set_Dead(true);
	return mrb_nil_value();
}


void SMC::Scripting::Init_Enemy(mrb_state* p_state)
{
	p_rcEnemy = mrb_define_class(p_state, "Enemy", p_rcAnimated_Sprite);
	MRB_SET_INSTANCE_TT(p_rcEnemy, MRB_TT_DATA);

	// For now, forbid creating generic enemies
	mrb_undef_class_method(p_state, p_rcEnemy, "new");

	mrb_define_method(p_state, p_rcEnemy, "kill!", Kill, ARGS_NONE());
}
