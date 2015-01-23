/***************************************************************************
 * animated_sprite.h
 *
 * Copyright © 2005 - 2011 Florian Richter
 * Copyright © 2013 - 2014 The TSC Contributors
 ***************************************************************************/
/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TSC_ANIMATED_SPRITE_HPP
#define TSC_ANIMATED_SPRITE_HPP

#include "../objects/movingsprite.hpp"
#include "../scripting/objects/sprites/mrb_animated_sprite.hpp"

namespace TSC {

    /* *** *** *** *** *** *** *** cAnimated_Sprite *** *** *** *** *** *** *** *** *** *** */

    class cAnimated_Sprite : public cMovingSprite {
    public:
        cAnimated_Sprite(cSprite_Manager* sprite_manager, std::string type_name = "sprite");
        virtual ~cAnimated_Sprite(void);

        // Create the MRuby instance for this object.
        virtual mrb_value Create_MRuby_Object(mrb_state* p_state)
        {
            return mrb_obj_value(Data_Wrap_Struct(p_state, mrb_class_get(p_state, "AnimatedSprite"), &Scripting::rtTSC_Scriptable, this));
        }
    };

    /* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace TSC

#endif

