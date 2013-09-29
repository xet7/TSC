#include "mrb_bonusbox.h"
#include "../../../objects/bonusbox.h"
#include "../../../level/level.h"
#include "../../../core/sprite_manager.h"
#include "mrb_box.h"

/**
 * Class BonusBox:
 *
 * Parent [Box](box.html)
 * {: .superclass}
 *
 * _Bonus Boxes_ contain powerups (or just look like
 * that).
 */

using namespace SMC;
using namespace SMC::Scripting;

// Extern
struct RClass* SMC::Scripting::p_rcBonus_Box = NULL;

/**
 * Method: BonusBox::new
 *
 * Creates a new BonusBox with the default values.
 */
static mrb_value Initialize(mrb_state* p_state, mrb_value self)
{
	cBonusBox* p_box = new cBonusBox(pActive_Level->m_sprite_manager);
	DATA_PTR(self) = p_box;
	DATA_TYPE(self) = &rtSMC_Scriptable;

	// This is a generated object
	p_box->Set_Spawned(true);

	// Let SMC manage the memory
	pActive_Level->m_sprite_manager->Add(p_box);

	return self;
}

/**
 * Method: BonusBox#bonus_type=
 *
 *   bonus_type=( type ) → type
 *
 * Specify the bonus box’ type.
 *
 * #### Parameters
 * [type]
 *   The bonus box’ new type. One of the following symbols:
 *   empty
 *   : See _undefined_.
 *
 *   fireplant
 *   : Box containing a fireplant if Maryo is big,
 *     a red mushroom otherwise.
 *
 *   goldpiece
 *   : Box containing a goldpiece. Specify the color
 *     via #goldcolor=.
 *
 *   mushroom_default:
 *   : Box containing the normal red mushroom.
 *
 *   mushroom_blue
 *   : Box containing the blue ice mushroom if Maryo is big,
 *     a red mushroom otherwise.
 *
 *   mushroom_ghost
 *   : Box containing a ghost mushroom.
 *
 *   mushroom_live_1
 *   : Box containing a 1-up mushroom.
 *
 *   mushroom_poison
 *   : Box containing a poison mushroom
 *
 *   mushroom_red:
 *   : See *mushroom_default*.
 *
 *   star
 *   : Box containing a star.
 *
 *   undefined
 *   : Empty box.
 */
static mrb_value Set_Bonus_Type(mrb_state* p_state, mrb_value self)
{
	mrb_sym type;
	mrb_get_args(p_state, "n", &type);
	std::string typestr(mrb_sym2name(p_state, type));

	SpriteType bonustype;
	if (typestr == "undefined" || typestr == "empty")
		bonustype = TYPE_UNDEFINED;
	else if (typestr == "mushroom_default" || typestr == "mushroom_red")
		bonustype = TYPE_MUSHROOM_DEFAULT;
	else if (typestr == "fireplant")
		bonustype = TYPE_FIREPLANT;
	else if (typestr == "mushroom_blue")
		bonustype = TYPE_MUSHROOM_BLUE;
	else if (typestr == "mushroom_ghost")
		bonustype = TYPE_MUSHROOM_GHOST;
	else if (typestr == "mushroom_live_1")
		bonustype = TYPE_MUSHROOM_LIVE_1;
	else if (typestr == "star")
		bonustype = TYPE_STAR;
	else if (typestr == "goldpiece")
		bonustype = TYPE_GOLDPIECE;
	else if (typestr == "mushroom_poison")
		bonustype = TYPE_MUSHROOM_POISON;
	else {
		mrb_raisef(p_state, MRB_ARGUMENT_ERROR(p_state), "Invalid bonus box type %s", typestr.c_str());
		return mrb_nil_value(); // Not reached
	}

	cBonusBox* p_box = Get_Data_Ptr<cBonusBox>(p_state, self);
	p_box->Set_Bonus_Type(bonustype);

	return mrb_nil_value();
}

/**
 * Method: BonusBox#bonus_type
 *
 * Return the box’ current bonus type.
 */
static mrb_value Get_Bonus_Type(mrb_state* p_state, mrb_value self)
{
	cBonusBox* p_box = Get_Data_Ptr<cBonusBox>(p_state, self);
	switch(p_box->box_type) {
	case TYPE_UNDEFINED:
		return str2sym(p_state, "undefined");
	case TYPE_MUSHROOM_DEFAULT:
		return str2sym(p_state, "mushroom_default");
	case TYPE_FIREPLANT:
		return str2sym(p_state, "fireplant");
	case TYPE_MUSHROOM_BLUE:
		return str2sym(p_state, "mushroom_blue");
	case TYPE_MUSHROOM_GHOST:
		return str2sym(p_state, "mushroom_ghost");
	case TYPE_MUSHROOM_LIVE_1:
		return str2sym(p_state, "mushroom_live_1");
	case TYPE_STAR:
		return str2sym(p_state, "star");
	case TYPE_GOLDPIECE:
		return str2sym(p_state, "goldpiece");
	case TYPE_MUSHROOM_POISON:
		return str2sym(p_state, "mushroom_poison");
	default:
		return mrb_nil_value();
	}
}

/**
 * Method: force_best_item=
 *
 *   force_best_item=( bool ) → bool
 *
 * With regard to #bonus_type=, force always the best possible
 * item to come out of the box, regardless of Maryo’s state.
 * That is, if you set #bonus_type= to :mushroom_blue, and
 * Maryo is small, AND you set this option, the box will
 * spit out a blue mushroom nevertheless.
 *
 * #### Parameters
 * bool
 * : Whether or not to force the best item.
 */
static mrb_value Set_Force_Best_Item(mrb_state* p_state, mrb_value self)
{
	mrb_bool force;
	mrb_get_args(p_state, "b", &force);

	cBonusBox* p_box = Get_Data_Ptr<cBonusBox>(p_state, self);
	p_box->Set_Force_Best_Item(force);

	return mrb_bool_value(force);
}

/**
 * Method: BonusBox#force_best_item?
 *
 *   force_best_item?() → true or false
 *
 * Returns `true` if this box always contains the best
 * possible item, `false` otherwise.
 */
static mrb_value Does_Force_Best_Item(mrb_state* p_state, mrb_value self)
{
	cBonusBox* p_box = Get_Data_Ptr<cBonusBox>(p_state, self);
	return mrb_bool_value(p_box->m_force_best_item);
}

/**
 * Method: BonusBox#goldcolor=
 *
 *   goldcolor=( color ) → color
 *
 * Specify the color for goldpieces in the box. Only useful
 * if you set #bonus_type to :goldpiece.
 *
 * #### Parameters
 * [color]
 *   The goldpiece color. One of `:default`, `:yellow`, `:red`.
 */
static mrb_value Set_Goldcolor(mrb_state* p_state, mrb_value self)
{
	mrb_sym color;
	mrb_get_args(p_state, "n", &color);
	std::string colorstr(mrb_sym2name(p_state, color));

	DefaultColor col;
	if (colorstr == "default")
		col = COL_DEFAULT;
	else if (colorstr == "yellow")
		col = COL_YELLOW;
	else if (colorstr == "red")
		col = COL_RED;
	else {
		mrb_raisef(p_state, MRB_ARGUMENT_ERROR(p_state), "Invalid bonus box goldcolor %s", colorstr.c_str());
		return mrb_nil_value();
	}

	cBonusBox* p_box = Get_Data_Ptr<cBonusBox>(p_state, self);
	p_box->Set_Goldcolor(col);

	return mrb_symbol_value(col);
}

/**
 * Method: BonusBox#goldcolor
 *
 *   goldcolor() → a_symbol
 *
 * Returns the color for goldpieces in this box. See
 * #goldcolor= for possible return values.
 */
static mrb_value Get_Goldcolor(mrb_state* p_state, mrb_value self)
{
	cBonusBox* p_box = Get_Data_Ptr<cBonusBox>(p_state, self);
	switch(p_box->m_gold_color) {
	case COL_DEFAULT:
		return str2sym(p_state, "default");
	case COL_YELLOW:
		return str2sym(p_state, "yellow");
	case COL_RED:
		return str2sym(p_state, "red");
	default:
		return mrb_nil_value();
	}
}

void SMC::Scripting::Init_BonusBox(mrb_state* p_state)
{
	p_rcBonus_Box = mrb_define_class(p_state, "BonusBox", p_rcBox);
	MRB_SET_INSTANCE_TT(p_rcBonus_Box, MRB_TT_DATA);

	mrb_define_method(p_state, p_rcBonus_Box, "initialize", Initialize, MRB_ARGS_NONE());
	mrb_define_method(p_state, p_rcBonus_Box, "bonus_type=", Set_Bonus_Type, MRB_ARGS_REQ(1));
	mrb_define_method(p_state, p_rcBonus_Box, "bonus_type", Get_Bonus_Type, MRB_ARGS_NONE());
	mrb_define_method(p_state, p_rcBonus_Box, "force_best_item=", Set_Force_Best_Item, MRB_ARGS_REQ(1));
	mrb_define_method(p_state, p_rcBonus_Box, "force_best_item?", Does_Force_Best_Item, MRB_ARGS_NONE());
	mrb_define_method(p_state, p_rcBonus_Box, "goldcolor=", Set_Goldcolor, MRB_ARGS_REQ(1));
	mrb_define_method(p_state, p_rcBonus_Box, "goldcolor", Get_Goldcolor, MRB_ARGS_NONE());
}
