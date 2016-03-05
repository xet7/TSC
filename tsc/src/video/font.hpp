/***************************************************************************
 * font.h
 *
 * Copyright © 2006 - 2011 Florian Richter
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

#ifndef TSC_FONT_HPP
#define TSC_FONT_HPP

#include "../core/global_basic.hpp"
#include "../video/img_manager.hpp"

namespace TSC {

    /* *** *** *** *** *** *** *** Font Manager class *** *** *** *** *** *** *** *** *** *** */

    class cFont_Manager {
    public:
        cFont_Manager(void);
        ~cFont_Manager(void);

        // Default font sizes
        static const int FONTSIZE_NORMAL = 20;
        static const int FONTSIZE_SMALL = 11;
        static const int FONTSIZE_VERYSMALL = 9;

        // initialization
        void Init(void);

        /// Queues text for rendering in the render queue. Use this
        /// to get your text onto the screen.
        void Queue_Text(const sf::Text& text);

        /// Update an sf::Text instance with its parameters so it
        /// is suitable for Queue_Text().
        void Prepare_SFML_Text(sf::Text& text, const std::string& str, float x, float y, int fontsize = FONTSIZE_NORMAL, const Color color = static_cast<uint8_t>(0), bool ignore_camera = false);

        // Renders the given text into a new surface
        //cGL_Surface* Render_Text(TTF_Font* font, const std::string& text, const Color color = static_cast<uint8_t>(0));

        // TTF loaded fonts
        sf::Font m_font_normal;
    };

    /* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

// Font Handler
    extern cFont_Manager* pFont;

    /* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace TSC

#endif
