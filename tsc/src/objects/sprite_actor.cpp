#include "../core/global_basic.hpp"
#include "../scripting/scriptable_object.hpp"
#include "actor.hpp"
#include "sprite_actor.hpp"

using namespace TSC;
namespace fs = boost::filesystem;

cSpriteActor::cSpriteActor(fs::path texture_path)
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
