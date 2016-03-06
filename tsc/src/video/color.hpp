/***************************************************************************
 * color.h
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

#ifndef TSC_COLOR_HPP
#define TSC_COLOR_HPP

namespace TSC {

    /* *** *** *** *** *** *** *** *** Color class *** *** *** *** *** *** *** *** *** */

    class Color {
    public:
        Color(void)
        {
            red = 0;
            green = 0;
            blue = 0;
            alpha = 255;
        }

        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        {
            red = r;
            green = g;
            blue = b;
            alpha = a;
        }

        Color(float r, float g, float b, float a = 1.0f)
        {
            red = static_cast<uint8_t>(r * 255);
            green = static_cast<uint8_t>(g * 255);
            blue = static_cast<uint8_t>(b * 255);
            alpha = static_cast<uint8_t>(a * 255);
        }

        Color(uint8_t grey)
        {
            red = grey;
            green = grey;
            blue = grey;
            alpha = 255;
        }

        // Returns it as SFML color.
        inline sf::Color Get_SFML_Color(void) const
        {
            return sf::Color(red, green, blue);
        }

        inline CEGUI::colour Get_cegui_Color(void) const
        {
            return CEGUI::colour(static_cast<float>(red) / 255, static_cast<float>(green) / 255, static_cast<float>(blue) / 255, static_cast<float>(alpha) / 255);
        }

        // += operator
        inline void operator += (const Color& c)
        {
            red += c.red;
            green += c.green;
            blue += c.blue;
            alpha += c.alpha;
        }

        // -= operator
        inline void operator -= (const Color& c)
        {
            red -= c.red;
            green -= c.green;
            blue -= c.blue;
            alpha -= c.alpha;
        }

        // + operator
        inline Color operator + (const Color& c) const
        {
            return Color(static_cast<uint8_t>(red + c.red), green + c.green, blue + c.blue, alpha + c.alpha);
        }

        // - operator
        inline Color operator - (const Color& c) const
        {
            return Color(static_cast<uint8_t>(red - c.red), green - c.green, blue - c.blue, alpha - c.alpha);
        }

        // assignment operator
        inline Color& operator = (const Color& c)
        {
            red = c.red;
            green = c.green;
            blue = c.blue;
            alpha = c.alpha;

            return *this;
        }

        // compare
        inline bool operator == (const Color& c) const
        {
            return red == c.red && green == c.green && blue == c.blue && alpha == c.alpha;
        }

        inline bool operator == (const sf::Color& c) const
        {
            return red == c.r && green == c.g && blue == c.b && alpha == c.a;
        }

        inline bool operator != (const Color& c) const
        {
            return !(operator == (c));
        }

        inline bool operator != (const sf::Color& c) const
        {
            return !(operator == (c));
        }

        uint8_t red, green, blue, alpha;
    };

// Generic Colors
    static const Color blue = Color(static_cast<uint8_t>(150), 200, 225);
    static const Color darkblue = Color(static_cast<uint8_t>(0), 0, 128);
    static const Color lightblue = Color(static_cast<uint8_t>(41), 167, 255);
    static const Color black = Color(static_cast<uint8_t>(0), 0, 0);
    static const Color blackalpha128 = Color(static_cast<uint8_t>(0), 0, 0, 128);
    static const Color blackalpha192 = Color(static_cast<uint8_t>(0), 0, 0, 192);
    static const Color white = Color(static_cast<uint8_t>(255), 255, 255);
    static const Color whitealpha128 = Color(static_cast<uint8_t>(255), 255, 255, 128);
    static const Color grey = Color(static_cast<uint8_t>(128), 128, 128);
    static const Color lightgrey = Color(static_cast<uint8_t>(64), 64, 64);
    static const Color lightgreyalpha64 = Color(static_cast<uint8_t>(64), 64, 64, 64);
    static const Color green = Color(static_cast<uint8_t>(0), 230, 0);
    static const Color lightgreen = Color(static_cast<uint8_t>(20), 253, 20);
    static const Color lightgreenalpha64 = Color(static_cast<uint8_t>(30), 230, 30, 64);
    static const Color yellow = Color(static_cast<uint8_t>(255), 245, 10);
    static const Color greenyellow = Color(static_cast<uint8_t>(154), 205, 50);
    static const Color darkgreen = Color(static_cast<uint8_t>(1), 119, 34);
    static const Color red = Color(static_cast<uint8_t>(250), 0, 0);
    static const Color lightred = Color(static_cast<uint8_t>(255), 40, 20);
    static const Color lila = Color(static_cast<uint8_t>(200), 0, 255);
    static const Color orange = Color(static_cast<uint8_t>(248), 191, 38);
    static const Color lightorange = Color(static_cast<uint8_t>(255), 220, 100);

    /* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace TSC

#endif
