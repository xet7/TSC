/***************************************************************************
 * img_manager.h
 *
 * Copyright © 2003 - 2011 Florian Richter
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

#ifndef TSC_IMG_MANAGER_HPP
#define TSC_IMG_MANAGER_HPP

namespace TSC {

    /* *** *** *** *** *** *** cImage_Manager *** *** *** *** *** *** *** *** *** *** *** */

    /**
     * This class manages the SFML textures. As loading a texture is
     * a performance-intense operation, the number of textures should
     * be kept as low as possible. Thus, you can just ask this class
     * for a texture, and if it is loaded already, it will be returned,
     * otherwise it will be loaded and then cached for later calls.
     */
    class cImage_Manager
    {
    public:
        cImage_Manager(void);
        virtual ~cImage_Manager(void);

        void Preload_Textures(std::function<void (unsigned int files_done, unsigned int files_total)> cb);
        sf::Texture& Get_Texture(const boost::filesystem::path& path);
    private:
        std::map<boost::filesystem::path, sf::Texture*> m_textures;
    };

    /* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace TSC

#endif
