#include "../core/global_basic.hpp"
#include "../core/global_game.hpp"
#include "../core/property_helper.hpp"
#include "../core/bintree.hpp"
#include "../scripting/scriptable_object.hpp"
#include "../core/file_parser.hpp"
#include "../video/img_settings.hpp"
#include "../video/img_manager.hpp"
#include "../level/level.hpp"
#include "actor.hpp"
#include "sprite_actor.hpp"

using namespace TSC;
namespace fs = boost::filesystem;

cSpriteActor::cSpriteActor(fs::path relative_texture_path)
    : cActor()
{
    m_rel_texture_path = relative_texture_path;
}

cSpriteActor::~cSpriteActor()
{
    //
}

void cSpriteActor::Added_To_Level(cLevel* p_level, const unsigned long& uid)
{
    cActor::Added_To_Level(p_level, uid);

    const cImage_Manager::ConfiguredTexture& txtinfo = p_level->Get_ImageManager()->Get_Texture(m_rel_texture_path);
    m_sprite.setTexture(*txtinfo.m_texture);
    txtinfo.m_settings->Apply(*this);
}

void cSpriteActor::Draw(sf::RenderWindow& stage) const
{
    cActor::Draw(stage);

    stage.draw(m_sprite, getTransform()); // <3 SFML. This allows us to apply the same transformations that affect the collision rectangle to also affect the sprite.
}
