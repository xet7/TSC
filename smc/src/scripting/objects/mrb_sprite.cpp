// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../scripting.h"
#include "mrb_sprite.h"
#include "mrb_eventable.h"
#include "../events/event.h"
#include "../../level/level.h"
#include "../../core/sprite_manager.h"
#include "../../level/level_player.h"

/**
 * Class: Sprite
 *
 * A _Sprite_ is one of the most generic objects available in the MRuby
 * API. Anything shown on the screen is somehow a sprite, and the methods
 * defined in this class are therefore available to nearly all objects
 * exposed to the MRuby API.
 *
 * All sprites created by the regular SMC editor can be references by
 * indexing the global `UIDS` table, see
 * [Unique Identifiers](index.html#unique-identifiers-uids) for more
 * information on this topic.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ruby
 * UIDS[38].x
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * About coordinates
 * -----------------
 *
 * Instances of `Sprite` and its subclasses have two kinds of
 * coordinates, the normal ones (usually called just "coordinates") and
 * the starting position’s coordinates (usually called "initial
 * coordinates"). Most sprites don’t really care about the initial
 * coordinates, but some instances of subclasses of `Sprite` do, e.g. the
 * [flyon](flyon.html) remembers its starting position, i.e. where to
 * return after jumping out, in its initial coordinates.
 *
 * Events
 * ------
 *
 * Touch
 * : This event is fired when the sprite collides with another
 *   sprite. Note that such a collision actually creates two Touch
 *   events, one for each sprite. This may be useful if you don’t want to
 *   use a long `if-elsif-elsif-elsif...` construct inside an event
 *   handler choosing an action depending on the collided sprite.
 *
 *   The event handler gets passed an instance of this class (or one of
 *   its subclasses) representing the other collision "partner".
 */

using namespace SMC;
using namespace SMC::Scripting;

// Extern
struct RClass* SMC::Scripting::p_rcSprite     = NULL;
struct mrb_data_type SMC::Scripting::rtSprite = {"Sprite", NULL};

MRUBY_IMPLEMENT_EVENT(touch);

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

/**
 * Method: Sprite#show
 *
 *   show()
 *
 * Makes a sprite visible. See also [hide()](#hide).
 */
mrb_value Show(mrb_state* p_state, mrb_value self)
{
	cSprite* p_sprite = NULL;
	Data_Get_Struct(p_state, self, &rtSprite, p_sprite);
	p_sprite->Set_Active(true);
	return mrb_nil_value();
}

/**
 * Method: Sprite#hide
 *
 *   hide()
 *
 * Makes a sprite invisible. See also [show()](#show).
 */
mrb_value Hide(mrb_state* p_state, mrb_value self)
{
	cSprite* p_sprite = NULL;
	Data_Get_Struct(p_state, self, &rtSprite, p_sprite);
	p_sprite->Set_Active(false);
	return mrb_nil_value();
}

/**
 * Method: Sprite#uid
 *
 *   uid() → an_integer
 *
 * Returns the UID for the sprite.
 */
mrb_value Get_UID(mrb_state* p_state, mrb_value self)
{
	cSprite* p_sprite = NULL;
	Data_Get_Struct(p_state, self, &rtSprite, p_sprite);

	return mrb_fixnum_value(p_sprite->m_uid);
}

/**
 * Method: Sprite#x
 *
 *   x() → an_integer
 *
 * The current X coordinate.
 */
mrb_value Get_X(mrb_state* p_state, mrb_value self)
{
	cSprite* p_sprite = NULL;
	Data_Get_Struct(p_state, self, &rtSprite, p_sprite);

	return mrb_fixnum_value(p_sprite->m_pos_x);
}

/**
 * Method: Sprite#y
 *
 *   y() → an_integer
 *
 * The current Y coordinate.
 */
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

	mrb_define_method(p_state, p_rcSprite, "on_touch", MRUBY_EVENT_HANDLER(touch), ARGS_NONE());
}
