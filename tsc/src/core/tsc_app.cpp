#include "global_basic.hpp"
#include "global_game.hpp"
#include "errors.hpp"
#include "property_helper.hpp"
#include "xml_attributes.hpp"
#include "../scripting/scriptable_object.hpp"
#include "../objects/actor.hpp"
#include "../scenes/scene.hpp"
#include "../scenes/menu_scene.hpp"
#include "scene_manager.hpp"
#include "filesystem/resource_manager.hpp"
#include "../video/img_manager.hpp"
#include "filesystem/package_manager.hpp"
#include "i18n.hpp"
#include "../user/preferences.hpp"
#include "tsc_app.hpp"

TSC::cApp* TSC::gp_app = NULL;

using namespace TSC;

cApp::cApp()
{
    debug_print("Initializing application.\n");

    // init random number generator
    srand(static_cast<unsigned int>(time(NULL)));

    Init_SFML();
    Init_Managers();
    Init_User_Preferences();
    Init_I18N();
    Init_CEGUI();
}

cApp::~cApp()
{
    delete mp_scene_manager;
    delete mp_package_manager;
    delete mp_image_manager;
    delete mp_resource_manager;
    delete mp_preferences;
    delete mp_renderwindow;
}

void cApp::Init_SFML()
{
    debug_print("Initializing SFML.\n");
    mp_renderwindow = new sf::RenderWindow(sf::VideoMode(1024, 768), CAPTION);
}

void cApp::Init_Managers()
{
    debug_print("Initializing manager classes.\n");

    mp_resource_manager = new cResource_Manager();
    mp_resource_manager->Init_User_Directory();

    mp_image_manager = new cImage_Manager();
    mp_package_manager = new cPackage_Manager(*mp_resource_manager);
    mp_scene_manager = new cSceneManager();
}

void cApp::Init_User_Preferences()
{
    mp_preferences = cPreferences::Load_From_File(mp_resource_manager->Get_Preferences_File());
    debug_print("Configuration file is '%s'.\n", path_to_utf8(mp_preferences->m_config_filename).c_str());

    // mp_preferences->Apply();
}

void cApp::Init_I18N()
{
    debug_print("Initializing I18n.\n");

    // set game language
    I18N_Set_Language(mp_preferences->m_language);
    // init translation support
    I18N_Init(mp_resource_manager->Get_Game_Translation_Directory());
}

void cApp::Init_CEGUI()
{

}

/**
 * Initiate the main loop.
 */
int cApp::Run()
{
    // Set default package
    mp_package_manager->Set_Current_Package(mp_preferences->m_package);

    // Preload often used textures
    mp_image_manager->Preload_Textures(
        [](unsigned int files_done, unsigned int files_total){ /* Nothing right now */ }
    );

    // Always start with the start menu scene
    cMenuScene* p_menuscene = new cMenuScene();
    mp_scene_manager->Push_Scene(p_menuscene);

    // Start the main loop
    mp_scene_manager->Play(*mp_renderwindow);

    mp_preferences->Save();

    // TODO: Proper return value
    return 0;
}
