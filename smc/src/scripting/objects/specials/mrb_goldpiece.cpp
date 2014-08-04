#include "../../../objects/goldpiece.hpp"
#include "../../../core/sprite_manager.hpp"
#include "../../../level/level.hpp"
#include "mrb_goldpiece.hpp"
#include "../sprites/mrb_animated_sprite.hpp"
#include "../../events/event.hpp"

/**
 * Class: Goldpiece
 *
 * Parent: [AnimatedSprite](animatedsprite.html)
 * {: .superclass}
 *
 * _Goldpieces_ (or rather waffles in new versions of SMC).
 *
 * Note that this class describes the goldpieces you can also
 * directly place via the level editor’s "special" category, i.e.
 * the goldpieces don’t move. You can employ
 * one of the subclasses to get a different behaviour.
 *
 * Events
 * ------
 *
 * Activate
 * : This event is fired when the player collects the coin. Do
 *   not change the player’s gold amount inside an event handler
 *   for this event, this will cause undefined behaviour.
 */

using namespace SMC;
using namespace SMC::Scripting;


MRUBY_IMPLEMENT_EVENT(activate);

/**
 * Method: Goldpiece::new
 *
 *   new() → a_goldpiece
 *
 * Creates a new instance of this class with the default
 * values as per SMC’s internal code.
 */
static mrb_value Initialize(mrb_state* p_state, mrb_value self)
{
	cGoldpiece* p_gp = new cGoldpiece(pActive_Level->m_sprite_manager);
	DATA_PTR(self) = p_gp;
	DATA_TYPE(self) = &rtSMC_Scriptable;

	p_gp->Set_Spawned(true);
	pActive_Level->m_sprite_manager->Add(p_gp);

	return self;
}

/**
 * Method: Goldpiece#gold_color=
 *
 *   gold_color=( sym ) → sym
 *
 * Set the goldpiece’s color.
 *
 * #### Parameters
 *
 * sym
 * : The new goldpiece color. One of the symbols `:red` or `:yellow`.
 */
static mrb_value Set_Gold_Color(mrb_state* p_state, mrb_value self)
{
	mrb_sym colsym;
	mrb_get_args(p_state, "n", &colsym);
	string colorstr(mrb_sym2name(p_state, colsym));

	DefaultColor col;
	if (colorstr == "yellow")
		col = COL_YELLOW;
	else if (colorstr == "red")
		col = COL_RED;
	else {
		mrb_raisef(p_state, MRB_ARGUMENT_ERROR(p_state), "Invalid goldpiece color %s", colorstr.c_str());
		return mrb_nil_value(); // Not reached
	}

	cGoldpiece* p_gp = Get_Data_Ptr<cGoldpiece>(p_state, self);
	p_gp->Set_Gold_Color(col);

	return mrb_symbol_value(colsym);
}

/**
 * Method: Goldpiece#gold_color
 *
 *   gold_color() → a_symbol
 *
 * Returns the goldpiece’s current color. See #gold_color= for a
 * list of possible return values.
 */
static mrb_value Get_Gold_Color(mrb_state* p_state, mrb_value self)
{
	cGoldpiece* p_gp = Get_Data_Ptr<cGoldpiece>(p_state, self);

	switch(p_gp->m_color_type) {
	case COL_YELLOW:
		return str2sym(p_state, "yellow");
	case COL_RED:
		return str2sym(p_state, "red");
	default:
		return  mrb_nil_value();
	}
}

/**
 * Method: Goldpiece#activate
 *
 *   activate()
 *
 * Apply the goldpiece to the player.
 */
static mrb_value Activate(mrb_state* p_state, mrb_value self)
{
	cGoldpiece* p_gp = Get_Data_Ptr<cGoldpiece>(p_state, self);
	p_gp->Activate();
	return mrb_nil_value();
}

void SMC::Scripting::Init_Goldpiece(mrb_state* p_state)
{
	struct RClass* p_rcGoldpiece = mrb_define_class(p_state, "Goldpiece", mrb_class_get(p_state, "AnimatedSprite"));
	MRB_SET_INSTANCE_TT(p_rcGoldpiece, MRB_TT_DATA);

	mrb_define_method(p_state, p_rcGoldpiece, "initialize", Initialize, MRB_ARGS_NONE());
	mrb_define_method(p_state, p_rcGoldpiece, "gold_color=", Set_Gold_Color, MRB_ARGS_REQ(1));
	mrb_define_method(p_state, p_rcGoldpiece, "gold_color", Get_Gold_Color, MRB_ARGS_NONE());
	mrb_define_method(p_state, p_rcGoldpiece, "activate", Activate, MRB_ARGS_NONE());

	mrb_define_method(p_state, p_rcGoldpiece, "on_activate", MRUBY_EVENT_HANDLER(activate), MRB_ARGS_BLOCK());
}
