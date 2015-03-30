#include "../core/global_basic.hpp"
#include "../core/errors.hpp"
#include "../core/property_helper.hpp"
#include "../core/xml_attributes.hpp"
#include "../scripting/scriptable_object.hpp"
#include "../objects/actor.hpp"
#include "../scenes/scene.hpp"
#include "../core/scene_manager.hpp"
#include "../core/filesystem/resource_manager.hpp"
#include "../video/img_manager.hpp"
#include "../core/filesystem/package_manager.hpp"
#include "../user/preferences.hpp"
#include "../core/tsc_app.hpp"
#include "../objects/actor.hpp"
#include "scene.hpp"
#include "menu_scene.hpp"

using namespace TSC;

cMenuScene::cMenuScene()
{
    sf::Texture& menu_start_texture = gp_app->Get_ImageManager().Get_Texture(gp_app->Get_ResourceManager().Get_Game_Pixmap("menu/start.png"));
    sf::Texture& menu_quit_texture  = gp_app->Get_ImageManager().Get_Texture(gp_app->Get_ResourceManager().Get_Game_Pixmap("menu/quit.png"));

    m_menuitem_start.setTexture(menu_start_texture);
    m_menuitem_quit.setTexture(menu_quit_texture);

    m_menuitem_start.setPosition(sf::Vector2f(100, 100));
    m_menuitem_quit.setPosition(sf::Vector2f(100, 300));
}

cMenuScene::~cMenuScene()
{
    //
}

void cMenuScene::Handle_Event(sf::Event& evt)
{
    cScene::Handle_Event(evt);
}

void cMenuScene::Update()
{
    cScene::Update();
}

void cMenuScene::Draw(sf::RenderWindow& stage)
{
    cScene::Draw(stage);

    stage.draw(m_menuitem_start);
    stage.draw(m_menuitem_quit);
}
