// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../scripting.h"
#include "mrb_sprite.h"
#include "mrb_eventable.h"
#include "../../level/level.h"
#include "../../core/sprite_manager.h"
#include "../../level/level_player.h"

using namespace SMC;
using namespace SMC::Scripting;

// Extern
struct RClass* SMC::Scripting::p_rcSprite     = NULL;
struct mrb_data_type SMC::Scripting::rtSprite = {"Sprite", NULL};

static mrb_value Initialize(mrb_state* p_state, mrb_value self)
{
	mrb_int uid = -1;
	char* path = NULL;
	mrb_get_args(p_state, "|zi", &path, &uid);

	// Insert a new sprite instance into the MRuby object
	cSprite* p_sprite = new cSprite(pActive_Level->m_sprite_manager);
	DATA_PTR(self) = p_sprite;
	DATA_TYPE(self) = &rtSprite;

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

mrb_value Show(mrb_state* p_state, mrb_value self)
{
	cSprite* p_sprite = NULL;
	Data_Get_Struct(p_state, self, &rtSprite, p_sprite);
	p_sprite->Set_Active(true);
	return mrb_nil_value();
}

mrb_value Hide(mrb_state* p_state, mrb_value self)
{
	cSprite* p_sprite = NULL;
	Data_Get_Struct(p_state, self, &rtSprite, p_sprite);
	p_sprite->Set_Active(false);
	return mrb_nil_value();
}

mrb_value Get_UID(mrb_state* p_state, mrb_value self)
{
	cSprite* p_sprite = NULL;
	Data_Get_Struct(p_state, self, &rtSprite, p_sprite);

	return mrb_fixnum_value(p_sprite->m_uid);
}

mrb_value Get_X(mrb_state* p_state, mrb_value self)
{
	cSprite* p_sprite = NULL;
	Data_Get_Struct(p_state, self, &rtSprite, p_sprite);

	return mrb_fixnum_value(p_sprite->m_pos_x);
}

mrb_value Get_Y(mrb_state* p_state, mrb_value self)
{
	cSprite* p_sprite = NULL;
	Data_Get_Struct(p_state, self, &rtSprite, p_sprite);

	return mrb_fixnum_value(p_sprite->m_pos_y);
}

void SMC::Scripting::Init_Sprite(mrb_state* p_state)
{
	p_rcSprite = mrb_define_class(p_state, "Sprite", p_state->object_class);
	mrb_include_module(p_state, p_rcSprite, p_rmEventable);
	MRB_SET_INSTANCE_TT(p_rcSprite, MRB_TT_DATA);

	mrb_define_method(p_state, p_rcSprite, "initialize", Initialize, ARGS_OPT(2));
	mrb_define_method(p_state, p_rcSprite, "show", Show, ARGS_NONE());
	mrb_define_method(p_state, p_rcSprite, "hide", Hide, ARGS_NONE());
	mrb_define_method(p_state, p_rcSprite, "uid", Get_UID, ARGS_NONE());
	mrb_define_method(p_state, p_rcSprite, "x", Get_X, ARGS_NONE());
	mrb_define_method(p_state, p_rcSprite, "y", Get_Y, ARGS_NONE());
}
