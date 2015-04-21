#include "global_basic.hpp"
#include "global_game.hpp"
#include "../scripting/scriptable_object.hpp"
#include "../objects/actor.hpp"
#include "collision.hpp"

using namespace TSC;

cCollision::cCollision(cActor* p_collision_causer, cActor* p_collision_sufferer)
    : mp_collision_causer(p_collision_causer), mp_collision_sufferer(p_collision_sufferer)
{
    // Ensure we only compute the collision rectangles once so we can use them
    // for this objects methods. Performance.
    m_causer_colrect = mp_collision_causer->Get_Transformed_Collision_Rect();
    m_sufferer_colrect = mp_collision_sufferer->Get_Transformed_Collision_Rect();
}

/**
 * Invert the collision causer and suffer of this collision.
 */
void cCollision::Invert()
{
    cActor* p_temp = mp_collision_causer;
    mp_collision_causer = mp_collision_sufferer;
    mp_collision_sufferer = p_temp;

    sf::FloatRect temprect = m_causer_colrect;
    m_causer_colrect = m_sufferer_colrect;
    m_sufferer_colrect = temprect;
}

/**
 * Determine top collision. This method returns true if
 * the moving rectangle’s top edge hit the suffering
 * rectangle’s bottom edge.
 *
 * the suffering collision
 * rectangle was hit at the top by the causing rectangle.
 */
bool cCollision::Is_Collision_Top() const
{
    // Ensure X is in range and not the rectangles far apart.
    if (m_causer_colrect.left < m_sufferer_colrect.left + m_sufferer_colrect.width
        && m_causer_colrect.left + m_causer_colrect.width > m_sufferer_colrect.left) {

        // The top of the causer colrect must be above the bottom of the sufferer colrect
        // and the bottom of the causer colrect must be below the bottom of the sufferer colrect.
        if (m_causer_colrect.top < m_sufferer_colrect.top + m_sufferer_colrect.height
            && m_causer_colrect.top + m_causer_colrect.height > m_sufferer_colrect.top + m_sufferer_colrect.height) {
            return true;
        }
    }

    return false;
}

/**
 * Determine bottom collision. This method returns true if
 * the causing rectangle’s bottom edge hit the suffering
 * rectangle’s top edge.
 */
bool cCollision::Is_Collision_Bottom() const
{
    // Ensure X is in range and not the rectangles far apart.
    if (m_causer_colrect.left < m_sufferer_colrect.left + m_sufferer_colrect.width
        && m_causer_colrect.left + m_causer_colrect.width > m_sufferer_colrect.left) {

        // The bottom of the causer colrect must be below the top of the sufferer colrect
        // and the top of the causer colrect must be above the top of the sufferer colrect.
        if (m_causer_colrect.top + m_causer_colrect.height > m_sufferer_colrect.top
            && m_causer_colrect.top < m_sufferer_colrect.top) {
            return true;
        }
    }

    return false;
}

/**
 * Determine left collision. This method returns true if the
 * causing rectangle’s left edge has hit the suffering
 * rectangle’s right edge.
 */
bool cCollision::Is_Collision_Left() const
{
    // See Is_Collision_Bottom for analogous explanations for bottom collision.

    if (m_causer_colrect.top < m_sufferer_colrect.top + m_sufferer_colrect.height
        && m_causer_colrect.top + m_causer_colrect.height > m_sufferer_colrect.top) {

        if (m_causer_colrect.left < m_sufferer_colrect.left +  m_sufferer_colrect.width
            && m_causer_colrect.left + m_causer_colrect.width > m_sufferer_colrect.left + m_sufferer_colrect.width) {
            return true;
        }
    }

    return false;
}

/**
 * Determine right collision. This method returns true if
 * the causing rectangle’s right edge has hit the suffering
 * rectangle’s left side.
 */
bool cCollision::Is_Collision_Right() const
{
    // See Is_Collision_Top() for analogous explanations for top collision.

    if (m_causer_colrect.top < m_sufferer_colrect.top + m_sufferer_colrect.height
        && m_causer_colrect.top + m_causer_colrect.height > m_sufferer_colrect.top) {

        if (m_causer_colrect.left + m_causer_colrect.width > m_sufferer_colrect.left
            && m_causer_colrect.left < m_sufferer_colrect.left) {
            return true;
        }
    }

    return false;
}
