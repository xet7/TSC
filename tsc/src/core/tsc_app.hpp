/***************************************************************************
 * tsc_app.hpp
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

#ifndef TSC_APP_HPP
#define TSC_APP_HPP

namespace TSC {

    /**
     * TSC main application class. This is the top class for
     * managing everything. It’s a singleton, i.e. only instanciated
     * exactly once during the entire lifetime of the program.
     *
     * In the main() function the global variable TSC::gp_app is set
     * to the only instance of this class.
     */
    class cApp
    {
    public:
        cApp();
        ~cApp();

        int Run();

        inline sf::RenderWindow&  Get_RenderWindow(){return *mp_renderwindow;}
        inline cSceneManager&     Get_SceneManager(){return *mp_scene_manager;}
        inline cResource_Manager& Get_ResourceManager(){return *mp_resource_manager;}
        inline cPackage_Manager&  Get_PackageManager(){return *mp_package_manager;}

        inline cPreferences& Get_Preferences(){return *mp_preferences;}
    private:
        void Init_SFML();
        void Init_Managers();
        void Init_User_Preferences();
        void Init_I18N();
        void Init_CEGUI();

        cResource_Manager* mp_resource_manager;
        cSceneManager* mp_scene_manager;
        cPackage_Manager* mp_package_manager;

        cPreferences* mp_preferences;

        sf::RenderWindow* mp_renderwindow;
        CEGUI::OpenGLRenderer* mp_cegui_renderer;
        CEGUI::System* mp_cegui_system;
    };

    /**
     * Global variable pointing to the only cApp instance.
     */
    extern cApp* gp_app;
}

#endif
