// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../scripting.h"
#include "mrb_sprite.h"
#include "../../level/level.h"
#include "../../core/sprite_manager.h"
#include "../../level/level_player.h"

using namespace SMC;

static struct mrb_data_type sp_mruby_sprite_type_info = {"Sprite", NULL};

static mrb_value Initialize(mrb_state* p_state, mrb_value self)
{
	mrb_int uid = -1;
	char* path = NULL;
	mrb_get_args(p_state, "|zi", &path, &uid);

	// Insert a new sprite instance into the MRuby object
	cSprite* p_sprite = new cSprite(pActive_Level->m_sprite_manager);
	DATA_PTR(self) = p_sprite;
	DATA_TYPE(self) = &sp_mruby_sprite_type_info;

	// Arguments
	if (path)
		p_sprite->Set_Image(pVideo->Get_Surface(path), true);
	if (uid != -1) {
		if (pActive_Level->m_sprite_manager->Is_UID_In_Use(uid))
			mrb_raisef(p_state, MRB_ARGUMENT_ERROR(p_state), "UID %d is already used.", uid);

		p_sprite->m_uid = uid;
	}

	// Default massivity type is front passive
	p_sprite->Set_Sprite_Type(TYPE_FRONT_PASSIVE);

	// Hidden by default
	p_sprite->Set_Active(false);

	// This is a generated object that should neither be saved
	// nor should it be editable in the editor.
	p_sprite->Set_Spawned(true);

	// Add to the sprite manager for automatic memory management by SMC
	pActive_Level->m_sprite_manager->Add(p_sprite);

	return self;
}

void Scripting::Init_Sprite(mrb_state* p_state)
{
	struct RClass* p_Sprite = mrb_define_class(p_state, "Sprite", p_state->object_class);
	MRB_SET_INSTANCE_TT(p_Sprite, MRB_TT_DATA);

	mrb_define_method(p_state, p_Sprite, "initialize", Initialize, ARGS_OPT(2));
}
