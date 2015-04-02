#include "../core/global_basic.hpp"
#include "../scripting/scriptable_object.hpp"
#include "actor.hpp"
#include "static_actor.hpp"

using namespace TSC;
namespace fs = boost::filesystem;

cStaticActor::cStaticActor(fs::path texture_path)
{
    // Get Level manager, retrieve its Image manager, get the texture and
    // the settings, apply.
}

cStaticActor::~cStaticActor()
{
    //
}

void cStaticActor::Draw(sf::RenderWindow& stage) const
{
    cActor::Draw(stage);

    stage.draw(m_sprite, getTransform());
}
