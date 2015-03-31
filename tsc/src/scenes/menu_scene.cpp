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
    m_menuitem_options_texture.loadFromFile(gp_app->Get_ResourceManager().Get_Game_Pixmap("menu/options.png").native());
    m_menuitem_load_texture.loadFromFile(gp_app->Get_ResourceManager().Get_Game_Pixmap("menu/load.png").native());
    m_menuitem_save_texture.loadFromFile(gp_app->Get_ResourceManager().Get_Game_Pixmap("menu/save.png").native());
    m_menuitem_quit_texture.loadFromFile(gp_app->Get_ResourceManager().Get_Game_Pixmap("menu/quit.png").native());
    m_menuitem_start_texture.setSmooth(true);
    m_menuitem_options_texture.setSmooth(true);
    m_menuitem_load_texture.setSmooth(true);
    m_menuitem_save_texture.setSmooth(true);
    m_menuitem_quit_texture.setSmooth(true);

    m_menu_items.resize(MENUITEM_COUNT);

    m_menu_items[0].setTexture(m_menuitem_start_texture);
    m_menu_items[1].setTexture(m_menuitem_options_texture);
    m_menu_items[2].setTexture(m_menuitem_load_texture);
    m_menu_items[3].setTexture(m_menuitem_save_texture);
    m_menu_items[4].setTexture(m_menuitem_quit_texture);

    int x = calc_center_startx(100);
    m_menu_items[0].setPosition(sf::Vector2f(x, 100));
    m_menu_items[1].setPosition(sf::Vector2f(x, 200));
    m_menu_items[2].setPosition(sf::Vector2f(x, 300));
    m_menu_items[3].setPosition(sf::Vector2f(x, 400));
    m_menu_items[4].setPosition(sf::Vector2f(x, 500));

    m_menu_items[0].setScale(MIN_SCALE, MIN_SCALE);
    m_menu_items[1].setScale(MIN_SCALE, MIN_SCALE);
    m_menu_items[2].setScale(MIN_SCALE, MIN_SCALE);
    m_menu_items[3].setScale(MIN_SCALE, MIN_SCALE);
    m_menu_items[4].setScale(MIN_SCALE, MIN_SCALE);

    m_titlemusic.openFromFile(gp_app->Get_ResourceManager().Get_Game_Music(TITLE_MUSIC).native());
    m_titlemusic.setLoop(true);
    m_titlemusic.play();

    m_selected_item_id = 0;
    m_scale_factor = MIN_SCALE;
    m_is_growing = true;
}

cMenuScene::~cMenuScene()
{
    m_titlemusic.stop();
}

void cMenuScene::Handle_Event(sf::Event& evt)
{
    cScene::Handle_Event(evt);

    switch(evt.type) {
    case sf::Event::KeyReleased:
        on_handle_key_released(evt);
        break;
    default:
        break;
    }
}

void cMenuScene::Update()
{
    cScene::Update();

    if (m_is_growing) {
        m_scale_factor += 0.003f;

        if (m_scale_factor >= MAX_SCALE) {
            m_is_growing = false;
        }
    }
    else {
        m_scale_factor -= 0.003f;

        if (m_scale_factor <= MIN_SCALE) {
            m_is_growing = true;
        }
    }

    m_menu_items[m_selected_item_id].setScale(m_scale_factor, m_scale_factor);
}

void cMenuScene::Draw(sf::RenderWindow& stage)
{
    cScene::Draw(stage);

    std::vector<sf::Sprite>::const_iterator iter;
    for(iter=m_menu_items.begin(); iter != m_menu_items.end(); iter++)
        stage.draw(*iter);
}

void cMenuScene::on_handle_key_released(sf::Event& evt)
{
    switch (evt.key.code) {
    case sf::Keyboard::Down:
        m_selected_item_id++;

        if (m_selected_item_id >= MENUITEM_COUNT)
            m_selected_item_id = MENUITEM_COUNT - 1;

        break;
    case sf::Keyboard::Up:
        m_selected_item_id--;

        if (m_selected_item_id < 0)
            m_selected_item_id = 0;
        break;
    default:
        break;
    }

    std::vector<sf::Sprite>::iterator iter;
    for(iter=m_menu_items.begin(); iter != m_menu_items.end(); iter++)
        iter->setScale(MIN_SCALE, MIN_SCALE);
}

/**
 * Returns the start X position for centering an object of the given
 * width on the current window.
 */
int cMenuScene::calc_center_startx(int width)
{
    int screenwidth = gp_app->Get_Preferences().m_video_screen_w;
    return (screenwidth - width) / 2;
}
