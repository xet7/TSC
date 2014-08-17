#include "../collision.hpp"
#include "rect.hpp"
#include "circle.hpp"

using namespace SMC;

bool GL_Circle::Intersects(const GL_Circle& c) const
{
    return Col_Circle(m_x, m_y, m_radius, c.Get_X(), c.Get_Y(), c.Get_Radius());
}

bool GL_Circle::Intersects(const GL_rect& r) const
{
    return Col_Circle(*this, r);
}

/*bool GL_Circle::Intersects(const GL_point& p)
{
    // TODO
}
*/
