#include "../core/global_basic.hpp"
#include "../core/errors.hpp"
#include "../core/property_helper.hpp"
#include "../core/xml_attributes.hpp"
#include "../scripting/scriptable_object.hpp"
#include "../objects/actor.hpp"
#include "../scenes/scene.hpp"
#include "../core/scene_manager.hpp"
#include "../core/filesystem/resource_manager.hpp"
#include "../core/filesystem/package_manager.hpp"
#include "../user/preferences.hpp"
#include "../core/tsc_app.hpp"
#include "../objects/actor.hpp"
#include "scene.hpp"
#include "menu_scene.hpp"

using namespace TSC;

cMenuScene::cMenuScene()
{
    m_menuitem_start_texture.loadFromFile(gp_app->Get_ResourceManager().Get_Game_Pixmap("menu/start.png").native());
    m_menuitem_quit_texture.loadFromFile(gp_app->Get_ResourceManager().Get_Game_Pixmap("menu/quit.png").native());

    m_menuitem_start_texture.setSmooth(true);
    m_menuitem_quit_texture.setSmooth(true);

    m_menuitem_start.setTexture(m_menuitem_start_texture);
    m_menuitem_quit.setTexture(m_menuitem_quit_texture);

    m_menuitem_start.setPosition(sf::Vector2f(100, 100));
    m_menuitem_quit.setPosition(sf::Vector2f(100, 300));

    m_menuitem_start.setScale(0.1953f, 0.1953f);
    m_menuitem_quit.setScale(0.1953f, 0.1953f);
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
