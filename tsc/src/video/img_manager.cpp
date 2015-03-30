/***************************************************************************
 * img_manager.cpp  -  Texture Handler/Manager
 *
 * Copyright © 2003 - 2011 Florian Richter
 * Copyright © 2013 - 2015 The TSC Contributors
 ***************************************************************************/
/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

using namespace TSC;
namespace fs = boost::filesystem;

cImage_Manager::cImage_Manager()
{
    //
}

cImage_Manager::~cImage_Manager()
{
    std::map<fs::path, sf::Texture*>::iterator iter;
    for(iter=m_textures.begin(); iter != m_textures.end(); iter++)
        delete iter->second;
}

/**
 * Load a texture from the given file if it hasn’t been loaded
 * yet. If it has been loaded already, return it from the cache.
 */
sf::Texture& cImage_Manager::Get_Texture(const fs::path& path)
{
    if (m_textures.count(path)) {
        return *m_textures[path];
    }
    else {
        sf::Texture* p_texture = new sf::Texture;
        p_texture->loadFromFile(path_to_utf8(path.c_str())); // FIXME: Which encoding does SFML want here?
        m_textures[path] = p_texture;
        return *p_texture;
    }
}

/**
 * Preload the SFML textures for some heavily used images.
 * The function accepts a callback that receives the number of files
 * already processed and the total number of files intended to
 * be processed.
 */
void cImage_Manager::Preload_Textures(std::function<void (unsigned int files_done, unsigned int files_total)> cb)
{
    // image files
    vector<fs::path> image_files;

    // player
    vector<fs::path> player_small_images    = Get_Directory_Files(pResource_Manager->Get_Game_Pixmaps_Directory() / utf8_to_path("alex/small"), ".png", false, false);
    vector<fs::path> player_big_images      = Get_Directory_Files(pResource_Manager->Get_Game_Pixmaps_Directory() / utf8_to_path("alex/big"), ".png", false, false);
    vector<fs::path> player_fire_images     = Get_Directory_Files(pResource_Manager->Get_Game_Pixmaps_Directory() / utf8_to_path("alex/fire"), ".png", false, false);
    vector<fs::path> player_ice_images      = Get_Directory_Files(pResource_Manager->Get_Game_Pixmaps_Directory() / utf8_to_path("alex/ice"), ".png", false, false);
    vector<fs::path> player_ghost_images    = Get_Directory_Files(pResource_Manager->Get_Game_Pixmaps_Directory() / utf8_to_path("alex/ghost"), ".png", false, false);

    image_files.insert(image_files.end(), player_small_images.begin(), player_small_images.end());
    image_files.insert(image_files.end(), player_big_images.begin(), player_big_images.end());
    image_files.insert(image_files.end(), player_fire_images.begin(), player_fire_images.end());
    image_files.insert(image_files.end(), player_ice_images.begin(), player_ice_images.end());
    image_files.insert(image_files.end(), player_ghost_images.begin(), player_ghost_images.end());

    // Mushrooms
    image_files.push_back(utf8_to_path("game/items/mushroom_red.png"));
    image_files.push_back(utf8_to_path("game/items/mushroom_green.png"));
    image_files.push_back(utf8_to_path("game/items/mushroom_blue.png"));
    image_files.push_back(utf8_to_path("game/items/mushroom_ghost.png"));
    // Fireberry
    image_files.push_back(utf8_to_path("game/items/fireberry_1.png"));
    image_files.push_back(utf8_to_path("game/items/fireberry_2.png"));
    image_files.push_back(utf8_to_path("game/items/fireberry_3.png"));
    // Star
    image_files.push_back(utf8_to_path("game/items/lemon_1.png"));
    // Feather
    //image_files.push_back( "game/items/feather_1.png" );
    // Yellow Goldpiece
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/1.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/2.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/3.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/4.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/5.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/6.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/7.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/8.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/9.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/10.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/1_falling.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/2_falling.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/3_falling.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/4_falling.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/5_falling.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/6_falling.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/7_falling.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/8_falling.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/9_falling.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/yellow/10_falling.png"));
    // Red Goldpiece
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/1.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/2.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/3.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/4.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/5.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/6.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/7.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/8.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/9.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/10.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/1_falling.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/2_falling.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/3_falling.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/4_falling.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/5_falling.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/6_falling.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/7_falling.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/8_falling.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/9_falling.png"));
    image_files.push_back(utf8_to_path("game/items/goldpiece/red/10_falling.png"));

    // Brown Box
    image_files.push_back(utf8_to_path("game/box/brown1_1.png"));

    // Light animation
    image_files.push_back(utf8_to_path("animation/light_1/1.png"));
    image_files.push_back(utf8_to_path("animation/light_1/2.png"));
    image_files.push_back(utf8_to_path("animation/light_1/3.png"));
    // Particle animations
    image_files.push_back(utf8_to_path("animation/particles/fire_1.png"));
    image_files.push_back(utf8_to_path("animation/particles/fire_2.png"));
    image_files.push_back(utf8_to_path("animation/particles/fire_3.png"));
    image_files.push_back(utf8_to_path("animation/particles/fire_4.png"));
    image_files.push_back(utf8_to_path("animation/particles/smoke.png"));
    image_files.push_back(utf8_to_path("animation/particles/smoke_black.png"));
    image_files.push_back(utf8_to_path("animation/particles/light.png"));
    image_files.push_back(utf8_to_path("animation/particles/dirt.png"));
    image_files.push_back(utf8_to_path("animation/particles/ice_1.png"));
    image_files.push_back(utf8_to_path("animation/particles/cloud.png"));
    image_files.push_back(utf8_to_path("animation/particles/axis.png"));

    // Ball
    image_files.push_back(utf8_to_path("animation/fireball/1.png"));
    image_files.push_back(utf8_to_path("animation/iceball/1.png"));

    // HUD
    image_files.push_back(utf8_to_path("game/alex_l.png"));
    image_files.push_back(utf8_to_path("game/gold_m.png"));
    image_files.push_back(utf8_to_path("game/itembox.png"));

    unsigned int loaded_files = 0;
    unsigned int file_count = image_files.size();

    // load images
    for (vector<fs::path>::iterator itr = image_files.begin(); itr != image_files.end(); ++itr) {
        // get filename
        fs::path filename = (*itr);

        // preload (cache) image
        get_texture(filename);

        // count files
        loaded_files++;

        // Callback
        cb(loaded_files, file_count);
    }
}
