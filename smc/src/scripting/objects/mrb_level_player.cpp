// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "mrb_level_player.h"
#include "mrb_animated_sprite.h"
#include "../../level/level_player.h"
#include "../../gui/hud.h"
#include "../events/event.h"

using namespace SMC;

// Extern
struct RClass* SMC::Scripting::p_rcLevel_Player     = NULL;
struct mrb_data_type SMC::Scripting::rtLevel_Player = {"LevelPlayer", NULL};

/**
 * Class: LevelPlayer
 *
 * The sole instance of this class, the singleton `Player`, represents
 * Maryo himself. Naturally you can’t instanciate this class (SMC isn’t a
 * multiplayer game), but otherwise this class is your interface to doing
 * all kinds of evil things with Maryo. You should, however, be careful,
 * because the powerful methods exposed by this class allow you to
 * control nearly every aspect of Maryo--if you exaggerate, the player
 * will get annoyed, probably stopping to play your level.
 *
 * This class’ documentation uses two words that you’re better off not
 * mixing up:
 *
 * Maryo (or just "the level player")
 * : The sprite of Maryo walking around and jumping on enemies.
 *
 * Player
 * : The actual user sitting in front of some kind of monitor. Don’t
 *   confuse him with Maryo, because a) he will probably get angry, and b)
 *   you’ll get funny meanings on sentences like "the player presses the
 *   _jump_ key". The only exception to this rule is the `Player`
 *   constant in Mruby, which obviously represents Maryo, not the guy
 *   playing the game.
 *
 * Events
 * ------
 *
 * Downgrade
 * : Whenever Maryo gets hit (but not killed), this event is triggered.
 *   The event handler gets passed Maryo’s current downgrade count
 *   (which is always 1) and Maryo’s maximum downgrade count (which
 *   is always 2). As you can see, the arguments passed are not really
 *   useful and are just there for symmetry with some enemies’
 *   _Downgrade_ event handlers.
 *
 * Gold_100
 * : After Maryo has collected 100 gold pieces/waffles, this event
 *   is triggered. The event handler isn’t passed anything, but note
 *   that it is highly discouraged to alter Maryo’s amount of gold from
 *   within the event handler; this may lead to unexpected behaviour
 *   such as Maryo having more than 100 gold pieces after all operations
 *   regarding the amount of gold/waffles have finished or even endless
 *   loops as altering the gold/waffle amount may cause subsequent events
 *   of this type to be triggered.
 *
 * Jump
 * : This event is issued when the Maryo does a valid jump, i.e. the
 *   player presses the _Jump_ key and Maryo is currently in a state that
 *   actually allows him to jump. The event is triggered immediately
 *   before starting the jump.
 *
 * Shoot
 * : Fired when Maryo executes a valid shoot, either fireball or
 *   iceball. As with the _Jump_ event, this is only triggered when the
 *   player presses the _Shoot_ key and Maryo is currently in a state
 *   that allows him to shoot. Likewise, the event is triggered just
 *   prior to the actual shot. The event handler gets passed either the
 *   string `"ice"` when the player fired an iceball, or `"fire"` when it
 *   was a fireball.
 *
 * Waffles_100
 * : Synonym for the `Gold_100` event.
 */

/***************************************
 * Events
 ***************************************/

MRUBY_IMPLEMENT_EVENT(gold_100);

/***************************************
 * Methods
 ***************************************/

/**
 * Method: LevelPlayer#kill
 *
 *   kill!()
 *
 * Immediately sends Maryo to heaven (or to hell; it depends).
 */
static mrb_value Kill(mrb_state* p_state, mrb_value self)
{
	pLevel_Player->DownGrade(true);
	return mrb_nil_value();
}

/**
 * Method: LevelPlayer#add_lives
 *
 *   add_lives( lives ) → an_integer
 *
 * Add to the player’s current number of lives.
 *
 * #### Parameter
 * lives
 * : The lives to add. This number may be negative, but note that setting
 *   lives to 0 or less doesn’t kill the player immediately as this
 *   number is only checked when the player gets killed by some other
 *   force.
 *
 * #### Return value
 * The number of lives Maryo now has.
 */
static mrb_value Add_Lives(mrb_state* p_state, mrb_value self)
{
	mrb_int lives;
	mrb_get_args(p_state, "i", &lives);

	pHud_Lives->Add_Lives(lives);
	return mrb_fixnum_value(pLevel_Player->m_lives);
}

/***************************************
 * Entry point
 ***************************************/

void SMC::Scripting::Init_Level_Player(mrb_state* p_state)
{
	p_rcLevel_Player = mrb_define_class(p_state, "LevelPlayer", p_rcAnimated_Sprite);
	MRB_SET_INSTANCE_TT(p_rcLevel_Player, MRB_TT_DATA);

	// Make the Player global the only instance of LevelPlayer
	mrb_define_const(p_state, p_state->object_class, "Player", pLevel_Player->Create_MRuby_Object(p_state));

	// Forbid creating new instances of LevelPlayer
	mrb_undef_class_method(p_state, p_rcLevel_Player, "new");

	// Normal methods
	mrb_define_method(p_state, p_rcLevel_Player, "kill!", Kill, ARGS_NONE());
	mrb_define_method(p_state, p_rcLevel_Player, "add_lives", Add_Lives, ARGS_REQ(1));

	// Event handlers
	mrb_define_method(p_state, p_rcLevel_Player, "on_gold_100", MRUBY_EVENT_HANDLER(gold_100), ARGS_BLOCK());
}
