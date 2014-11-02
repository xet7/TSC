#include "../../scripting.hpp"
#include "mrb_moving_sprite.hpp"
#include "mrb_animated_sprite.hpp"

/**
 * Class: AnimatedSprite
 *
 * Parent: [MovingSprite](movingsprite.html)
 * {: .superclass}
 *
 * Currently there are no objects exposed to the MRuby API
 * that are _AnimatedSprites_ without being an instance
 * of [MovingSprite](movingsprite.html).
 */

using namespace TSC;
using namespace TSC::Scripting;


static mrb_value Initialize(mrb_state* p_state,  mrb_value self)
{
    mrb_raise(p_state, MRB_NOTIMP_ERROR(p_state), "Cannot create instances of this class.");
    return self; // Not reached
}

void TSC::Scripting::Init_Animated_Sprite(mrb_state* p_state)
{
    struct RClass* p_rcAnimated_Sprite = mrb_define_class(p_state, "AnimatedSprite", mrb_class_get(p_state, "MovingSprite"));

    mrb_define_method(p_state, p_rcAnimated_Sprite, "initialize", Initialize, MRB_ARGS_NONE());
}
