#include "../core/global_basic.hpp"
#include "../core/global_game.hpp"
#include "../core/bintree.hpp"
#include "../core/property_helper.hpp"
#include "../core/errors.hpp"
#include "../core/xml_attributes.hpp"
#include "../scripting/scriptable_object.hpp"
#include "../core/collision.hpp"
#include "../level/level.hpp"
#include "../objects/actor.hpp"
#include "../objects/sprite_actor.hpp"
#include "scene.hpp"
#include "../core/scene_manager.hpp"
#include "../core/filesystem/resource_manager.hpp"
#include "../core/filesystem/package_manager.hpp"
#include "../user/preferences.hpp"
#include "../core/tsc_app.hpp"
#include "level_scene.hpp"

using namespace TSC;
namespace fs = boost::filesystem;

cLevelScene::cLevelScene(const std::string& startlevel)
{
    Add_Level(cLevel::Construct_Debugging_Level());
}

cLevelScene::~cLevelScene()
{
    std::vector<cLevel*>::iterator iter;
    for(iter=m_active_levels.begin(); iter != m_active_levels.end(); iter++)
        delete *iter;
}

void cLevelScene::Handle_Event(sf::Event& evt)
{
    if (evt.type != sf::Event::KeyPressed)
        return;

    cLevel_Player* p_player = gp_current_level->Get_Player();

    switch (evt.key.code) {
    default:
        return;
    }
}

void cLevelScene::Update(sf::RenderWindow& stage)
{
    cScene::Update(stage);

    gp_current_level->Update();
    stage.setView(gp_current_level->Get_View());
}

void cLevelScene::Draw(sf::RenderWindow& stage)
{
    cScene::Draw(stage);
    // Draw the level elements themselves
    gp_current_level->Draw(stage);

    // Draw the HUD
    // TODO
}

std::string cLevelScene::Name() const
{
    return "LevelScene";
}

/**
 * Add the given level to the level scene and make it the current
 * level. The level will subsequently be memory-managed by this
 * class, so don’t `delete` it after you added it to the scene.
 */
void cLevelScene::Add_Level(cLevel* p_level)
{
    m_active_levels.push_back(p_level);
    gp_current_level = p_level;
}

/**
 * Add the level with the given name to the level scene and make it
 * the current level.
 *
 * \param[in] levelname
 * Name of the level to load. This is a level name, not a full
 * filename, i.e. something like "lvl_1" without directory
 * and extension. All the necessary pathes are searched automatically
 * to find the actual level file.
 */
void cLevelScene::Add_Level(const std::string& levelname)
{
    // Figure out the absolute path of this level.
    fs::path absolute_level_filename = gp_app->Get_PackageManager().Find_Level(levelname);

    if (absolute_level_filename.empty())
        throw(std::runtime_error("Level file not found!")); // TODO: Proper exception

    cLevel* p_level = cLevel::Load_From_File(absolute_level_filename);
    Add_Level(p_level);
}

/**
 * Set the current level to the loaded level with the given index.
 */
void cLevelScene::Set_Current_Level(size_t i)
{
    gp_current_level = m_active_levels[i];
}
