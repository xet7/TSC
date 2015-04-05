#include "../core/global_basic.hpp"
#include "../core/global_game.hpp"
#include "../core/property_helper.hpp"
#include "../core/bintree.hpp"
#include "../scripting/scriptable_object.hpp"
#include "../level/level.hpp"
#include "actor.hpp"
#include "sprite_actor.hpp"

using namespace TSC;
namespace fs = boost::filesystem;

cSpriteActor::cSpriteActor(fs::path texture_path, cLevel& level, unsigned long uid)
    : cActor(level, uid)
{
    // Get Level manager, retrieve its Image manager, get the texture and
    // the settings, apply.
}

cSpriteActor::~cSpriteActor()
{
    //
}

void cSpriteActor::Draw(sf::RenderWindow& stage) const
{
    cActor::Draw(stage);

    stage.draw(m_sprite, getTransform());
}
