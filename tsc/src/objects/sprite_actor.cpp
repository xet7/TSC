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

    m_pos_z = 0;
    m_z_layer = ZLAYER_PASSIVE;
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


/**
 * Calculate the Z coordinate for this actor. The Z ordering in TSC
 * works as follows: Each actor resides on a specific Z layer, determined
 * by its `m_z_layer` member. Inside this layer, the sprite has a Z coordinate,
 * defined by the `m_z_pos` member. By adding these two values together, the
 * final Z coordinate in the global actors list is found. The layers are not
 * kept strictly separate, so while unlikely, it is technically possible to
 * evolve from one layer to the next by incrementing `m_pos_z` enough. However,
 * iterating one single list is significantly faster than iterating one list
 * per layer, and it allows some actors to decide more freely about their Z
 * positioning; especially enemies do not necessarily fit into the classical
 * layer structure (think Gee vs. Flyon: Gee must be in front of a pipe
 * sprite, Flyon must be behind it).
 *
 * The final Z coordinate is what is returned by this method.
 */
float cSpriteActor::Z() const
{
    return m_z_layer + m_pos_z;
}
