// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "mrb_enemy.h"
#include "mrb_animated_sprite.h"
#include "../../enemies/enemy.h"
#include "../../gui/hud.h"

/**
 * Class: Enemy
 *
 * Parent: [AnimatedSprite](animatedsprite.html)
 * {: .superclass}
 *
 * _Enemies_ are the little guys that hang around all over the world of
 * SMC and try to hinder you in many different ways. This is the base
 * class for all enemies and defines the methods that are available for
 * every enemy, regardless of its type.
 *
 * Currently, this class is not instanciatable.
*/

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
 * Immediately kills this enemy. No points are given to
 * the player (i.e. it is as if the enemy fell into an abyss
 * or something like that).
 *
 * Causes a subsequent _Die_ event.
 *
 * See also: [#kill_with_points!](#killwithpoints).
 */
static mrb_value Kill(mrb_state* p_state,  mrb_value self)
{
	cEnemy* p_enemy = Get_Data_Ptr<cEnemy>(p_state, self);

	p_enemy->Set_Dead(true);
	return mrb_nil_value();
}

/**
 * Method: Enemy#kill_with_points!
 *
 *   kill_with_points!()
 *
 * Immediately kills this enemy. Points are given to the
 * player. Also plays the dying sound for this enemy.
 *
 * Causes a subsequent _Die_ event.
 *
 * See also: [#kill!](#kill)
 */
static mrb_value Kill_With_Points(mrb_state* p_state,  mrb_value self)
{
	cEnemy* p_enemy = Get_Data_Ptr<cEnemy>(p_state, self);

	pHud_Points->Add_Points(	p_enemy->m_kill_points,
								p_enemy->m_pos_x,
								p_enemy->m_pos_y - 5.0f);
	pAudio->Play_Sound(p_enemy->m_kill_sound);
	p_enemy->Set_Dead(true);

	return mrb_nil_value();
}

/**
 * Method: Enemy#kill_points
 *
 *   kill_points() → an_integer
 *
 * Returns the number of points the player gains after killing this
 * enemy.
 */
static mrb_value Get_Kill_Points(mrb_state* p_state,  mrb_value self)
{
	cEnemy* p_enemy = Get_Data_Ptr<cEnemy>(p_state, self);

	return mrb_fixnum_value(p_enemy->m_kill_points);
}

/**
 * Method: Enemy#kill_points=
 *
 *   kill_points=( points )
 *
 * Sets the number of points the player gains after killing this
 * enemy. Note kill points are not applied if the enemy dies due to
 * something else than player interaction, e.g. by falling into an
 * abyss.
 *
 * #### Parameters
 * points
 * : Number of points to add.
*/
static mrb_value Set_Kill_Points(mrb_state* p_state,  mrb_value self)
{
	cEnemy* p_enemy = Get_Data_Ptr<cEnemy>(p_state, self);
	mrb_int points;
	mrb_get_args(p_state, "i", &points);
	p_enemy->m_kill_points = points;

	return mrb_fixnum_value(points);
}

/**
 * Method: Enemy#kill_sound
 *
 *   kill_sound() → a_string
 *
 * Returns the filename of the sound to play when the enemy gets killed,
 * relative to the `sounds/` directory.
 */
static mrb_value Get_Kill_Sound(mrb_state* p_state,  mrb_value self)
{
	cEnemy* p_enemy = Get_Data_Ptr<cEnemy>(p_state, self);

	return mrb_str_new_cstr(p_state, p_enemy->m_kill_sound.c_str());
}

/**
 * Method: Enemy#kill_sound=
 *
 *   kill_sound=( path )
 *
 * Sets the sound to play when the enemy gets killed.
 *
 * #### Parameters
 * path
 * : The path of the sound file to play. Relative to the `sounds/`
 *   directory.
*/
static mrb_value Set_Kill_Sound(mrb_state* p_state,  mrb_value self)
{
	cEnemy* p_enemy = Get_Data_Ptr<cEnemy>(p_state, self);
	char* path;
	mrb_get_args(p_state, "z", &path);
	p_enemy->m_kill_sound = path;

	return mrb_str_new_cstr(p_state, path);
}

/**
 * Method: Enemy#fire_resistant=
 *
 *   fire_resistant=( val )
 *
 * Enables/Disables fire resistance for this enemy.
 *
 * #### Parameters
 * val
 * : `true` to enable the fire resistance, `false` to disable it.
*/
static mrb_value Set_Fire_Resistant(mrb_state* p_state,  mrb_value self)
{
	cEnemy* p_enemy = Get_Data_Ptr<cEnemy>(p_state, self);
	mrb_value obj;
	mrb_get_args(p_state, "o", &obj);
	p_enemy->m_fire_resistant = mrb_test(obj) ? true : false;

	return obj;
}

/**
 * Method: Enemy#fire_resistant?
 *
 *   fire_resistant?
 *
 * Checks whether this enemy is resistant to fire.
 */
static mrb_value Is_Fire_Resistant(mrb_state* p_state,  mrb_value self)
{
	cEnemy* p_enemy = Get_Data_Ptr<cEnemy>(p_state, self);

	return p_enemy->m_fire_resistant ? mrb_true_value() : mrb_false_value();
}


void SMC::Scripting::Init_Enemy(mrb_state* p_state)
{
	p_rcEnemy = mrb_define_class(p_state, "Enemy", p_rcAnimated_Sprite);
	MRB_SET_INSTANCE_TT(p_rcEnemy, MRB_TT_DATA);

	// For now, forbid creating generic enemies
	mrb_undef_class_method(p_state, p_rcEnemy, "new");

	mrb_define_method(p_state, p_rcEnemy, "kill_points", Get_Kill_Points, ARGS_NONE());
	mrb_define_method(p_state, p_rcEnemy, "kill_points=", Set_Kill_Points, ARGS_REQ(1));
	mrb_define_method(p_state, p_rcEnemy, "kill_sound", Get_Kill_Sound, ARGS_NONE());
	mrb_define_method(p_state, p_rcEnemy, "kill_sound=", Set_Kill_Sound, ARGS_REQ(1));
	mrb_define_method(p_state, p_rcEnemy, "fire_resistant=", Set_Fire_Resistant, ARGS_REQ(1));
	mrb_define_method(p_state, p_rcEnemy, "fire_resistant?", Is_Fire_Resistant, ARGS_NONE());
	mrb_define_method(p_state, p_rcEnemy, "kill!", Kill, ARGS_NONE());
	mrb_define_method(p_state, p_rcEnemy, "kill_with_points!", Kill_With_Points, ARGS_NONE());
}
