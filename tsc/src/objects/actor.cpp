#include "../core/global_basic.hpp"
#include "../scripting/scriptable_object.hpp"
#include "actor.hpp"

using namespace TSC;

cActor::cActor()
    : cScriptable_Object()
{
    // Some sensible defaults for a collision rectangle so it’s not invisible
    // on debugging if unset.
    m_collision_rect.left = 0;
    m_collision_rect.top = 0;
    m_collision_rect.width = 100;
    m_collision_rect.height = 100;

    m_name = "(Unnamed actor)";
}

cActor::~cActor()
{
}

/**
 * Update this actor for the upcoming frame drawing.
 */
void cActor::Update()
{
    // Virtual
}

/**
 * Draw this actor onto the given window.
 */
void cActor::Draw(RenderTarget& stage)
{
    // Virtual
}


/**
 * Retrieves the collision rect how it looks after being transformed.
 * Note SFML only supports 90° step rotations on rectangles so until
 * someone implements this in our code, no finer rotation resolution
 * is possible. If you try, you’ll get the bounding rectangle of the
 * transformed rectangle back. Other transformations should work fine.
 *
 * \see [sf::Transform::transformRect()](http://www.sfml-dev.org/documentation/2.2/classsf_1_1Transform.php#a345112559981d988e92b54b7976fca8a)
 */
sf::FloatRect cActor::Get_Transformed_Collision_Rect() const
{
    return getTransform().transformRect(m_collision_rect);
}

/**
 * Checks if this actor’s collision rectangle collides with
 * the given other rectangle. Rectangles collide if they intersect.
 */
bool cActor::Does_Collide(const sf::FloatRect& other_rect) const
{
    return Get_Transformed_CollisionRect().intersects(other_rect);
}

/**
 * Checks if this actor’s collision rectangle collides with
 * the given other point. A point collides with a rectangle if
 * it is contained in it.
 */
bool cActor::Does_Collide(const sf::Vector2f& other_point) const
{
    return Get_Transformed_Collision_Rect().contains(other_point);
}

/**
 * Checks if this actor’s collision rectangle collides with
 * the given other actor’s collision rectangle. Two rectangles
 * collide if they intersect.
 */
bool cActor::Does_Collide(const cActor& other_actor) const
{
    return Get_Transformed_Collision_Rect().intersects(other_actor.Get_Transformed_Collision_Rect());
}