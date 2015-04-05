#include "global_basic.hpp"
#include "collision.hpp"

using namespace TSC;

cCollision::cCollision(cActor& collision_causer, cActor& collision_sufferer)
    : m_collision_causer(collision_causer), m_collision_sufferer(collision_sufferer)
{
    // Ensure we only compute the collision rectangles once so we can use them
    // for this objects methods. Performance.
    m_causer_colrect = m_collision_causer.Get_Transformed_Collision_Rect();
    m_sufferer_colrect = m_collision_sufferer.Get_Transformed_Collision_Rect();
}

/**
 * Determine top collision. This method returns true if the suffering collision
 * rectangle was hit at the top by the causing rectangle.
 */
bool cCollision::Is_Collision_Top() const
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
 * Determine bottom collision. This method returns true if the suffering collision
 * rectangle was hit at the bottom by the causing rectangle.
 */
bool cCollision::Is_Collision_Bottom() const
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
 * Determine left collision. This method returns true if the
 * suffering collision rectangle was hit at the left side by
 * the causing rectangle.
 */
bool cCollision::Is_Collision_Left() const
{
    // See Is_Collision_Top() for analogous explanations for top collision.

    if (m_causer_colrect.top < m_sufferer_colrect.top + m_sufferer_colrect.height
        && m_causer_colrect.top + m_causer_colrect.height > m_sufferer_colrect.top) {

        if (m_causer_colrect.left + m_causr_colrect.width > m_sufferer_colrect.left
            && m_causer_colrect.left < m_sufferer_colrect.left) {
            return true;
        }
    }

    return false;
}

/**
 * Determine right collision. This method returns true if
 * the suffering collision rectangle was hit at the right
 * side by the causing rectangle.
 */
bool cCollision::Is_Collision_Right() const
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
