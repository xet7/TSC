#include "../core/global_basic.hpp"
#include "../scripting/scriptable_object.hpp"
#include "actor.hpp"

using namespace TSC;

cActor::cActor()
    : cScriptable_Object()
{
    //
}

cActor::~cActor()
{
}

void cActor::Update()
{
}

void cActor::Draw(sf::RenderWindow& stage)
{
    stage.draw(m_sprite);
}

/**
 * Specify the visual dimensions of the actor. This also resets
 * the collision rectangle to exactly the same dimension; use
 * Set_Collision_Rect() to specify another collision rectangle.
 */
void cActor::Set_Dimensions(int width, int height)
{
    sf::FloatRect normal_rect = m_sprite.getLocalBounds();
    m_sprite.setScale(width / normal_rect.width, height / normal_rect.height);

    m_collision_rect = sf::FloatRect(0, 0, width, height);
}
