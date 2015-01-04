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
#include "../core/game_core.hpp"
#include "../core/framerate.hpp"
#include "../core/file_parser.hpp"
#include "../core/filesystem/package_manager.hpp"
#include "../core/global_basic.hpp"

using namespace std;

namespace TSC {

/* *** *** *** *** *** *** *** cAnimation_Parser *** *** *** *** *** *** *** *** *** *** */
class cAnimation_Parser : public cFile_parser
{
public:
    typedef std::pair<std::string, Uint32> Entry_Type;
    typedef std::vector<Entry_Type> List_Type;

    cAnimation_Parser(Uint32 time) : m_time(time)
    {
    }

    bool HandleMessage(const std::string* parts, unsigned int count, unsigned int line)
    {
        if(count == 1)
        {
            // only a filename
            m_images.push_back(Entry_Type(parts[0], m_time));
        }
        else if(count == 2)
        {
            if(parts[0] == "time")
            {
                // Setting default time for images
                m_time = string_to_int(parts[1]);
            }
            else
            {
                // filename and a time for that specific image
                m_images.push_back(Entry_Type(parts[0], string_to_int(parts[1])));
            }
        }

        return 1;
    }


    List_Type m_images;
    Uint32 m_time;
};


/* *** *** *** *** *** *** *** cAnimation_Surface *** *** *** *** *** *** *** *** *** *** */

cAnimation_Surface::cAnimation_Surface(void)
{
    m_image = NULL;
    m_time = 0;
}

cAnimation_Surface::~cAnimation_Surface(void)
{
    //
}

/* *** *** *** *** *** *** *** cAnimated_Sprite *** *** *** *** *** *** *** *** *** *** */

cAnimated_Sprite::cAnimated_Sprite(cSprite_Manager* sprite_manager, std::string type_name /* = "sprite" */)
    : cMovingSprite(sprite_manager, type_name)
{
    m_curr_img = -1;

    m_anim_enabled = 0;
    m_anim_img_start = 0;
    m_anim_img_end = 0;
    m_anim_time_default = 1000;
    m_anim_counter = 0;
    m_anim_mod = 1.0f;
}

cAnimated_Sprite::~cAnimated_Sprite(void)
{
    //
}

void cAnimated_Sprite::Add_Image(cGL_Surface* image, Uint32 time /* = 0 */)
{
    // set to default time
    if (time == 0) {
        time = m_anim_time_default;
    }

    cAnimation_Surface obj;
    obj.m_image = image;
    obj.m_time = time;

    m_images.push_back(obj);
}

void cAnimated_Sprite::Add_Animation(const std::string& name, boost::filesystem::path path, Uint32 time /* = 0 */)
{
    // Parse the animation file
    boost::filesystem::path filename = pPackage_Manager->Get_Pixmap_Reading_Path(path_to_utf8(path));
    if(filename == boost::filesystem::path())
    {
        cerr << "Warning: Unable to load animation: " << name << endl;
        return;
    }

    cAnimation_Parser parser(time == 0 ? m_anim_time_default : time);
    if(!parser.Parse(path_to_utf8(filename)))
    {
        cerr << "Warning: Unable to parse animation file: " << filename << endl;
        return;
    }

    if(parser.m_images.size() == 0)
    {
        cerr << "Warning: Empty animation file: " << filename << endl;
        return;
    }

    // Add images
    int start, end;
    start = m_images.size();
    for(cAnimation_Parser::List_Type::iterator itr = parser.m_images.begin(); itr != parser.m_images.end(); ++itr)
    {
        Add_Image(pVideo->Get_Package_Surface(path.parent_path() / utf8_to_path(itr->first)), itr->second);
    }
    end = m_images.size() - 1;

    // Add the item
    m_named_ranges[name] = std::pair<int, int>(start, end);
}

void cAnimated_Sprite::Set_Named_Animation(const std::string& name)
{
    cAnimation_Name_Map::iterator it = m_named_ranges.find(name);
    if(it == m_named_ranges.end())
    {
        cerr << "Warning: Named animation not found: " << name << endl;
        Set_Image_Num(-1);
        Set_Animation(0);
        return;
    }

    int start = it->second.first;
    int end = it->second.second;

    Set_Image_Num(start);
    if(end > start)
    {
        Set_Animation(1);
        Set_Animation_Image_Range(start, end);
        Reset_Animation();
    }
    else
    {
        Set_Animation(0);
        Reset_Animation();
    }
}

void cAnimated_Sprite::Set_Image_Num(const int num, const bool new_startimage /* = 0 */, const bool del_img /* = 0 */)
{
    if (m_curr_img == num) {
        return;
    }

    m_curr_img = num;

    if (m_curr_img < 0) {
        cMovingSprite::Set_Image(NULL, new_startimage, del_img);
    }
    else if (m_curr_img < static_cast<int>(m_images.size())) {
        cMovingSprite::Set_Image(m_images[m_curr_img].m_image, new_startimage, del_img);
    }
    else {
        debug_print("Warning : Object image number %d bigger as the array size %u, sprite type %d, name %s\n", m_curr_img, static_cast<unsigned int>(m_images.size()), m_type, m_name.c_str());
    }
}

cGL_Surface* cAnimated_Sprite::Get_Image(const unsigned int num) const
{
    if (num >= m_images.size()) {
        return NULL;
    }

    return m_images[num].m_image;
}

void cAnimated_Sprite::Clear_Images(void)
{
    m_curr_img = -1;
    m_images.clear();
    m_named_ranges.clear();
}

void cAnimated_Sprite::Update_Animation(void)
{
    // if not valid
    if (!m_anim_enabled || m_anim_img_end == 0) {
        return;
    }

    m_anim_counter += pFramerate->m_elapsed_ticks;

    // out of range
    if (m_curr_img < 0 || m_curr_img >= static_cast<int>(m_images.size())) {
        cerr << "Warning: Animation image " << m_curr_img << " for " << m_name << " out of range (max " << (m_images.size() - 1) << "). Forcing start image." << endl;
        Set_Image_Num(m_anim_img_start);
        return;
    }

    cAnimation_Surface image = m_images[m_curr_img];

    if (static_cast<Uint32>(m_anim_counter * m_anim_mod) >= image.m_time) {
        if (m_curr_img >= m_anim_img_end) {
            Set_Image_Num(m_anim_img_start);
        }
        else {
            Set_Image_Num(m_curr_img + 1);
        }

        m_anim_counter = static_cast<Uint32>(m_anim_counter * m_anim_mod) - image.m_time;
    }
}

void cAnimated_Sprite::Set_Time_All(const Uint32 time, const bool default_time /* = 0 */)
{
    for (cAnimation_Surface_List::iterator itr = m_images.begin(); itr != m_images.end(); ++itr) {
        cAnimation_Surface& obj = (*itr);
        obj.m_time = time;
    }

    if (default_time) {
        Set_Default_Time(time);
    }
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace TSC
