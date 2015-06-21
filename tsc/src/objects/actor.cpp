#include "../core/global_basic.hpp"
#include "../core/global_game.hpp"
#include "../core/bintree.hpp"
#include "../core/errors.hpp"
#include "../core/property_helper.hpp"
#include "../core/xml_attributes.hpp"
#include "../core/math/utilities.hpp"
#include "../scripting/scriptable_object.hpp"
#include "../objects/actor.hpp"
#include "../scenes/scene.hpp"
#include "../core/scene_manager.hpp"
#include "../core/filesystem/resource_manager.hpp"
#include "../video/img_manager.hpp"
#include "../core/filesystem/package_manager.hpp"
#include "../user/preferences.hpp"
#include "../core/tsc_app.hpp"
#include "../level/level.hpp"
#include "../core/errors.hpp"
#include "../core/xml_attributes.hpp"
#include "actor.hpp"

using namespace TSC;

/**
 * Construct a new actor with the default values.
 *
 * \returns New cActor instance.
 */
cActor::cActor()
    : cScriptable_Object()
{
    // Some sensible defaults for a collision rectangle so it’s not invisible
    // on debugging if unset.
    m_collision_rect.left = 0;
    m_collision_rect.top = 0;
    m_collision_rect.width = 100;
    m_collision_rect.height = 100;
    // Ensure these are set to the same defaults as the colrect above
    m_debug_colrect_shape.setPosition(sf::Vector2f(0, 0));
    m_debug_colrect_shape.setSize(sf::Vector2f(100, 100));

    // Color for the debug colrect
    m_debug_colrect_shape.setFillColor(sf::Color(255, 255, 0, 100));

    m_name = "(Unnamed actor)";

    /* Invisible actors must be drawn at the very front in editor
     * mode. In ordinary gameplay, the Draw() function does nothing,
     * so these high values don’t hurt. In subclasses that are
     * visible, you want to obviously adjust these values. */
    m_pos_z = 999.99;
    m_z_layer = ZLAYER_FRONTPASSIVE;

    // Invisible objects should not hinder gameplay by default, subclasses
    // of invisible objecta can of course behave different (e.g. cEnemyStopper).
    m_coltype = COLTYPE_PASSIVE;

    // By default, invisible objects are not subject to gravity.
    m_gravity_max = 0;
    m_gravity_accel = 0;
    mp_ground_object = NULL;
    m_ground_type = GROUND_NORMAL;

    m_uid = 0;
    mp_level = NULL;
}

cActor::cActor(XmlAttributes& attributes, cLevel& level, const std::string type_name)
{
    // TODO
}


cActor::~cActor()
{
}

cActor* cActor::Copy(void) const
{
    cActor* actor = new cActor();
    actor->Set_Pos(m_start_pos.x, m_start_pos.y, true);
    actor->m_uid = 0; // Do not copy UID, must be unique

    return actor;
}

/**
 * Method for initiating the actor’s update. This method does the following
 * things:
 *
 * 1. Call Update_Gravity()
 * 2. Call Update(), the virtual function you override in your subclasses.
 * 3. Call Update_Position()
 *
 * This method is not intended to be overridden, override Update() instead.
 */
void cActor::Do_Update()
{
    Update_Gravity();
    Update();
    Update_Position();
}

/**
 * Update this actor for the upcoming frame drawing. By default, this
 * does nothing. Override it in your subclasses. Note the position
 * in the update chain where this is called. This position is explained
 * in Do_Update(), but note especially that the actor’s velocity is set
 * according to the gravity for this frame, but not applied yet so that
 * you can modify it to your likening. *After* this method returns,
 * position updating happens.
 */
void cActor::Update()
{
    // Virtual
}

/**
 * Calculate and apply the gravity effect on this object, increasing
 * its vertical downwards velocity if it doesn’t stand on a colliding
 * object. This method does not change the object’s actual positioning,
 * this is left to Update_Position().
 */
void cActor::Update_Gravity()
{
    // Shortcut if this object is not subject to gravity at all
    if (Is_Float_Equal(m_gravity_accel, 0.0f))
        return;
    // Shortcut if we stand on a ground object and can’t even fall.
    if (mp_ground_object)
        return;

    // Accelerate falling by the gravity factor.
    Accelerate_Y(m_gravity_accel);

    // "Aerial resistance" stops incrementing of the falling velocity some time.
    if (m_velocity.y > m_gravity_max)
        m_velocity.y = m_gravity_max;
}

/**
 * Apply the velocity found in `m_velocity` without any restrictions, moving
 * the actor visibily on the screen (when it’s drawn next time).
 *
 * Issues a check for collisions with this object.
 */
void cActor::Update_Position()
{
    // Shortcut if nothing to do
    if (m_velocity.x == 0 && m_velocity.y == 0)
        return;

    // SFML transformation
    move(m_velocity);

    // Check for collisions if this is an object that can collide.
    if (m_coltype != COLTYPE_PASSIVE)
        mp_level->Check_Collisions_For_Actor(*this);

    // TODO: Check level edges
}

/**
 * Draw this actor onto the given window. By default, draws
 * the collision rect in debug mode.
 */
void cActor::Draw(sf::RenderWindow& stage) const
{
    if (gp_app->Is_Debug_Mode()) {
        stage.draw(m_debug_colrect_shape, getTransform());
    }
}

/**
 * “Callback” function called when this actor is added to a level.
 * You can override this in your subclasses, but be sure to call
 * the parent class’ method, because cActor::Added_To_Level() takes
 * care of setting the `mp_level` and `m_uid` members correctly.
 *
 * \param level
 * The level this actor have been added to.
 *
 * \param[uid]
 * The UID this actor was assigned in the level.
 */
void cActor::Added_To_Level(cLevel* p_level, const unsigned long& uid)
{
    m_uid = uid;
    mp_level = p_level;
}

/**
 * Accelerate in → direction.
 *
 * \param deltax
 * Difference to move.
 *
 * \param real
 * (false) Normally, the global speedfactor is applied to the `deltax`,
 * which is the (highly) recommended behaviour to cause the same effect
 * regardless of the framerate. If you set this parameter to true,
 * `deltax` is interpreted as an absolute pixel value instead.
 */
void cActor::Accelerate_X(const float& deltax, bool real /* = false */)
{
    if (real) {
        m_velocity.x += deltax;
    }
    else {
        m_velocity.x += deltax * gp_app->Get_SceneManager().Get_Speedfactor();
    }
}

/**
 * Accelerate in ↓ direction.
 *
 * \param deltay
 * Difference to move.
 *
 * \param real
 * (false) Normally, the global speedfactor is applied to the `deltay`,
 * which is the (highly) recommended behaviour to cause the same effect
 * regardless of the framerate. If you set this parameter to true,
 * `deltay` is interpreted as an absolute pixel value instead.
 */
void cActor::Accelerate_Y(const float& deltay, bool real /* = false */)
{
    if (real) {
        m_velocity.y += deltay;
    }
    else {
        m_velocity.y += deltay * gp_app->Get_SceneManager().Get_Speedfactor();
    }
}

/**
 * Accelerate in → and ↓ direction.
 *
 * \param deltax
 * Difference in X direction.
 *
 * \param deltay
 * Difference in Y direction.
 *
 * \param real
 * (false) Normally, the global speedfactor is applied to
 * `deltax` and `deltay`, which is the (highly) recommended behaviour
 * to cause the same effect regardless of the framerate. If you set
 * this parameter to true, `deltax` and `deltay` are interpreted as an
 * absolute pixel value instead.
 */
void cActor::Accelerate_XY(const float& deltax, const float& deltay, bool real /* = false */)
{
    if (real) {
        m_velocity.x += deltax;
        m_velocity.y += deltay;
    }
    else {
        m_velocity.x += deltax * gp_app->Get_SceneManager().Get_Speedfactor();
        m_velocity.y += deltay * gp_app->Get_SceneManager().Get_Speedfactor();
    }
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
    return Get_Transformed_Collision_Rect().intersects(other_rect);
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
float cActor::Z() const
{
    return m_z_layer + m_pos_z;
}

/**
 * Set the collision rectangle on this actor. This method circumvents
 * the position transformation setting, so use with care: Always assume
 * that your actor is at position (0|0) and not scaled, rotated, etc.
 * other than the ersult of the settings file’s `width` and `height`
 * keys! This actor usual transformations will be applied to this
 * collision rectangle subsequently. I.e. you set the rectangle BEFORE
 * any transformation, except the width/height keys from the settings files!
 *
 * \param rect
 * The new collision rectangle.
 */
void cActor::Set_Collision_Rect(sf::FloatRect rect)
{
    m_collision_rect = rect;
    m_debug_colrect_shape.setPosition(sf::Vector2f(rect.left, rect.top));
    m_debug_colrect_shape.setSize(sf::Vector2f(rect.width, rect.height));
}

/**
 * Handle a collision with another actor. This method is intended to
 * be overridden in subclasses. The actor that this method is called
 * on always is the *causer* member of the collision object passed
 * as a parameter, i.e. you use cCollision::Get_Collision_Sufferer()
 * to get the other collision partner. Even if you return false
 * from this method (see below), the collision is *inversed* so that
 * the collision partner now receives the collision with himself
 * set to be the causer.
 *
 * By default, this method does nothing and returns true, i.e. it
 * “swallows” the collision.
 *
 * \param[in] p_collision
 * The collision object.
 *
 * \returns Return true from this method if you have handled the
 * collision. If you return false, the collision will be inverted
 * and then passed to Handle_Collision() on the collision partner
 * (if that partner returns false also, nothing happens).
 */
bool cActor::Handle_Collision(cCollision* p_collision)
{
    return true;
}

/**
 * Compare two actors. Two actors are equal if:
 *
 * 1. They have the same UID.
 * 2. They belong to the same level.
 */
bool cActor::operator==(const cActor& other) const
{
    if (other.m_uid != m_uid)
        return false;

    if (*other.mp_level == *mp_level)
        return true;
    else
        return false;
}

/**
 * Inverse of operator==().
 */
bool cActor::operator!=(const cActor& other) const
{
    return !(*this == other);
}

/**
 * Set this actor onto an object below so it stands on it.
 * Gravity will not affect the actor while he stands on that
 * object. Calling this method will also immediately reset
 * the Y velocity to zero, i.e. stop any falling.
 *
 * \param[in] p_ground_object
 * Object to stand on.
 */
void cActor::Set_On_Ground(cActor* p_ground_object)
{
    mp_ground_object = p_ground_object;
    m_velocity.y = 0; // Imagine a big crater here…
}

/**
 * Take away the ground object from the actor’s feet. The
 * actor will be subject to gravity again after this method
 * has been called. Has no effect if the actor is not standing
 * on anything.
 *
 * \returns The previous ground object. NULL if there was none,
 * i.e. the actor was falling already.
 */
cActor* cActor::Reset_On_Ground()
{
    cActor* ptr = mp_ground_object;
    mp_ground_object = NULL;
    return ptr;
}

void cActor::Auto_Slow_Down(float x_speed, float y_speed /* = 0 */)
{
    // horizontal slow down
    if (x_speed > 0.0f) {
        if (m_velocity.x > 0.0f) {
            Add_Velocity_X_Min(-x_speed, 0.0f);
        }
        else if (m_velocity.x < 0.0f) {
            Add_Velocity_X_Max(x_speed, 0.0f);
        }
    }

    // vertical slow down
    if (y_speed > 0.0f) {
        if (m_velocity.y > 0.0f) {
            Add_Velocity_Y_Min(-y_speed, 0.0f);
        }
        else if (m_velocity.y < 0.0f) {
            Add_Velocity_Y_Max(y_speed, 0.0f);
        }
    }
}
