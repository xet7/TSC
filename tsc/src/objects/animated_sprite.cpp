/***************************************************************************
 * animated_sprite.cpp  - multi image object sprite class
 *
 * Copyright © 2005 - 2011 The TSC Contributors
 ***************************************************************************/
/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../objects/animated_sprite.hpp"

using namespace std;

namespace TSC {


/* *** *** *** *** *** *** *** cAnimated_Sprite *** *** *** *** *** *** *** *** *** *** */

cAnimated_Sprite::cAnimated_Sprite(cSprite_Manager* sprite_manager, std::string type_name /* = "sprite" */)
    : cMovingSprite(sprite_manager, type_name)
{
}

cAnimated_Sprite::~cAnimated_Sprite(void)
{
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace TSC
