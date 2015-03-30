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
sf::Texture& cImage_Manager::get_texture(const fs::path& path)
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
