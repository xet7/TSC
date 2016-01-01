/***************************************************************************
 * font.cpp  -  internal font functions
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

#include "../video/font.hpp"
#include "../video/gl_surface.hpp"
#include "../core/property_helper.hpp"
#include "../core/filesystem/resource_manager.hpp"
#include "../core/global_basic.hpp"

using namespace std;
using namespace TSC;

/* *** *** *** *** *** *** *** Font Manager class *** *** *** *** *** *** *** *** *** *** */

cFont_Manager::cFont_Manager(void)
{
    //
}

cFont_Manager::~cFont_Manager(void)
{
    //
}

void cFont_Manager::Init(void)
{
    if (!m_font_normal.loadFromFile(path_to_utf8(pResource_Manager->Get_Gui_Font_Directory() / utf8_to_path("default_bold.ttf")))) {
        // FIXME: Throw a proper exception
        throw "Font loading failed";
    }
}

// OLD cGL_Surface* cFont_Manager::Render_Text(TTF_Font* font, const std::string& text, const Color color)
// OLD {
// OLD     std::cerr << "WARNING: cFont_Manager::Render_Text() not yet ported to SFML." << std::endl;
// OLD 
// OLD     // HACK to satisfy return value
// OLD     return pVideo->Load_GL_Surface("game/arrow/small/blue/right.png");
// OLD 
// OLD     // OLD // get SDL Color
// OLD     // OLD SDL_Color sdlcolor = color.Get_SDL_Color();
// OLD     // OLD // create text surface
// OLD     // OLD cGL_Surface* surface = pVideo->Create_Texture(TTF_RenderUTF8_Blended(font, text.c_str(), sdlcolor));
// OLD     // OLD 
// OLD     // OLD if (!surface) {
// OLD     // OLD     return NULL;
// OLD     // OLD }
// OLD     // OLD 
// OLD     // OLD surface->m_path = utf8_to_path(text);
// OLD     // OLD 
// OLD     // OLD // set function if font gets deleted
// OLD     // OLD surface->Set_Destruction_Function(&Font_Delete_Ref);
// OLD     // OLD // add font to active fonts
// OLD     // OLD Add_Ref(surface);
// OLD     // OLD 
// OLD     // OLD return surface;
// OLD }

void cFont_Manager::Queue_Text(const std::string& text, float x, float y, int fontsize /* = FONTSIZE_NORMAL */, const Color color /* = black */)
{
    sf::Text* p_text = new sf::Text();

    p_text->setFont(m_font_normal);
    p_text->setColor(color.Get_SFML_Color());
    p_text->setCharacterSize(fontsize);
    p_text->setString(text);

    cText_Request* p_req = new cText_Request();
    p_req->mp_text = p_text;
    p_req->m_pos.x = x;
    p_req->m_pos.y = y;

    pRenderer->Add(p_req); // manages p_text
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

cFont_Manager* pFont = NULL;

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
