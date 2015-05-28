#include "../core/global_basic.hpp"
#include "../core/global_game.hpp"
#include "../core/property_helper.hpp"
#include "../scripting/scriptable_object.hpp"
#include "../core/file_parser.hpp"
#include "../video/img_set.hpp"
#include "../objects/actor.hpp"
#include "../objects/sprite_actor.hpp"
#include "../objects/animated_actor.hpp"
#include "../core/collision.hpp"
#include "../user/preferences.hpp"
#include "../core/scene_manager.hpp"
#include "../core/tsc_app.hpp"
#include "level_player.hpp"

using namespace TSC;
namespace fs = boost::filesystem;

cLevel_Player::cLevel_Player()
    : cAnimatedActor()
{
    m_name = "Alex";
    m_coltype = COLTYPE_MASSIVE;
    m_gravity_accel = 2.8f;
    m_gravity_max = 25.0f;

    m_state = STA_FALL;
    m_direction = DIR_RIGHT;

    m_god_mode = 0;

    m_walk_time = 0.0f;
    m_ghost_time = 0.0f;
    m_ghost_time_mod = 0.0f;

    // Starting with 3 lives
    m_lives = 3;
    m_goldpieces = 0;
    m_points = 0;
    m_kill_multiplier = 1.0f;
    m_last_kill_counter = 0.0f;

    // jump data
    m_up_key_time = 0.0f;
    m_force_jump = 0;
    m_next_jump_sound = 1;
    m_next_jump_power = 17.0f;
    m_next_jump_accel = 4.0f;
    m_jump_power = 0.0f;
    m_jump_accel_up = 4.5f;
    m_jump_vel_deaccel = 0.06f;

    // no movement timer
    m_no_velx_counter = 0.0f;
    m_no_vely_counter = 0.0f;

    m_shoot_counter = 0.0f;
    mp_active_object = NULL;
    m_duck_direction = DIR_UNDEFINED;

    m_is_warping = false;

    Add_Image_Set("small", "alex/small/walk.imgset");

    Set_Image_Set("small");
    Set_Image_Num(0, true);
}

cLevel_Player::~cLevel_Player()
{
    //
}

void cLevel_Player::Update()
{
    Update_Walking();
}

void cLevel_Player::Update_Walking(void)
{
    // OLD if (m_ducked_counter || !m_ground_object || (m_state != STA_STAY && m_state != STA_WALK && m_state != STA_RUN)) {
    // OLD    return;
    // OLD}

    // OLD // validate ground object
    // OLD if ((m_ground_object->m_type == TYPE_EATO || m_ground_object->m_type == TYPE_SPIKA || m_ground_object->m_type == TYPE_ROKKO || m_ground_object->m_type == TYPE_STATIC_ENEMY) && m_invincible <= 0 && !m_god_mode) {
    // OLD     Reset_On_Ground();
    // OLD }

    cPreferences& preferences = gp_app->Get_Preferences();
    if (sf::Keyboard::isKeyPressed(preferences.m_key_left) || sf::Keyboard::isKeyPressed(preferences.m_key_right) /* || TODO: joystick */) {
        float ground_mod = 1.0f;

        if (mp_ground_object) {
            // ground type
            switch (mp_ground_object->m_ground_type) {
            case GROUND_ICE:
                ground_mod = 0.5f;
                break;
            case GROUND_SAND:
                ground_mod = 0.7f;
                break;
            case GROUND_PLASTIC:
                ground_mod = 0.85f;
                break;
            default:
                break;
            }
        }

        Move_Player((0.6f * ground_mod * Get_Vel_Modifier()), (1.2f * ground_mod * Get_Vel_Modifier()));
    }

    // If the player moves faster than 10px/frame, upgrade his state to
    // running. Move_Player() above is responsible for setting the
    // velocity.
    if (m_state == STA_WALK) {
        // update
        if (m_velocity.x > 10.0f || m_velocity.x < -10.0f) {
            m_walk_time += gp_app->Get_SceneManager().Get_Speedfactor();

            if (m_walk_time > speedfactor_fps) {
                Set_Moving_State(STA_RUN);
            }
        }
        // reset
        else if (m_walk_time) {
            m_walk_time = 0.0f;
        }
    }
}

bool cLevel_Player::Handle_Collision(cCollision* p_collision)
{
    cAnimatedActor::Handle_Collision(p_collision);

    if (p_collision->Is_Collision_Bottom()) {
        Set_On_Ground(p_collision->Get_Collision_Sufferer());
        Set_Moving_State(STA_STAY);
    }

    return true;
}

void cLevel_Player::Move_Player(float velocity, float vel_wrongway)
{
    if (m_direction == DIR_LEFT) {
        velocity *= -1;
        vel_wrongway *= -1;
    }

    // OLD // get collision list
    // OLD cObjectCollisionType* col_list = Collision_Check_Relative(velocity, 0.0f, 0.0f, 0.0f, COLLIDE_ONLY_BLOCKING);
    // OLD // if collision with a blocking object
    // OLD bool is_col = 0;
    // OLD 
    // OLD // check collisions
    // OLD for (cObjectCollision_List::iterator itr = col_list->objects.begin(); itr != col_list->objects.end(); ++itr) {
    // OLD     cObjectCollision* col_obj = (*itr);
    // OLD 
    // OLD     // massive object is blocking
    // OLD     if (col_obj->m_obj->m_massive_type == MASS_MASSIVE) {
    // OLD         is_col = 1;
    // OLD         break;
    // OLD     }
    // OLD }
    // OLD 
    // OLD delete col_list;
    // OLD 
    // OLD // don't move if colliding
    // OLD if (is_col) {
    // OLD     if (Is_Float_Equal(m_velx, 0.0f)) {
    // OLD         Set_Moving_State(STA_STAY);
    // OLD     }
    // OLD     // slow down
    // OLD     else {
    // OLD         m_velx -= (m_velx * 0.1f) * pFramerate->m_speed_factor;
    // OLD 
    // OLD         if (m_velx > -0.1f && m_velx < 0.1f) {
    // OLD             m_velx = 0.0f;
    // OLD         }
    // OLD     }
    // OLD 
    // OLD     return;
    // OLD }

    // move right
    if (m_direction == DIR_RIGHT) {
        if (m_velocity.x > 0.0f) {
            Add_Velocity_X_Max(velocity, 10.0f * Get_Vel_Modifier());
        }
        else {
            // small smoke clouds under foots
            if (m_velocity.x < 0.0f) {
                // OLD Generate_Feet_Clouds();
            }

            // slow down
            Add_Velocity_X(vel_wrongway);
        }
    }
    // move left
    else if (m_direction == DIR_LEFT) {
        if (m_velocity.x < 0.0f) {
            Add_Velocity_X_Min(velocity, -10.0f * Get_Vel_Modifier());
        }
        else {
            // small smoke clouds under foots
            if (m_velocity.x > 0.0f) {
                // OLD Generate_Feet_Clouds();
            }

            // slow down
            Add_Velocity_X(vel_wrongway);
        }
    }

    // start walking
    if (m_state == STA_STAY) {
        Set_Moving_State(STA_WALK);
    }
}

float cLevel_Player::Get_Vel_Modifier(void) const
{
    float vel_mod = 1.0f;

    // if running key is pressed or always run
    cPreferences& preferences = gp_app->Get_Preferences();
    if (preferences.m_always_run || sf::Keyboard::isKeyPressed(preferences.m_key_action) /* || TODO: Joystick */) {
        vel_mod = 1.5f;
    }

    if (m_invincible_star > 0.0f) {
        vel_mod *= 1.2f;
    }

    if (m_state == STA_RUN) {
        vel_mod *= 1.2f;
    }

    return vel_mod;
}

void cLevel_Player::Set_Moving_State(Moving_state new_state)
{
    if (new_state == m_state) {
        return;
    }

    // can not change from linked state
    if (m_state == STA_OBJ_LINKED) {
        return;
    }

    // was falling
    if (m_state == STA_FALL) {
        // reset slow fall/parachute
        // OLD Parachute(0);
    }
    // was flying
    else if (m_state == STA_FLY) {
        // OLD Change_Size(0, -(m_images[ALEX_IMG_FALL].m_image->m_h - m_images[ALEX_IMG_FLY].m_image->m_h));
        // OLD Set_Image_Num(ALEX_IMG_FALL + m_direction);
        // OLD 
        // OLD // reset flying rotation
        // OLD m_rot_z = 0.0f;
    }
    // was jumping
    else if (m_state == STA_JUMP) {
        // reset variables
        m_up_key_time = 0.0f;
        m_jump_power = 0.0f;
        m_force_jump = 0;
        m_next_jump_sound = 1;
        m_next_jump_power = 17.0f;
        m_next_jump_accel = 4.0f;
    }
    // was running
    else if (m_state == STA_RUN) {
        m_walk_time = 0.0f;
        m_running_particle_counter = 0.0f;
    }

    // # before new state is set
    if (new_state == STA_STAY) {
        if (mp_ground_object) {
            m_velocity.y = 0.0f;
        }
    }
    else if (new_state == STA_FALL) {
        Reset_On_Ground();
        m_walk_count = 0.0f;
        m_jump_power = 0.0f;
    }
    else if (new_state == STA_FLY) {
        Reset_On_Ground();
    }
    else if (new_state == STA_JUMP) {
        Reset_On_Ground();
    }
    else if (new_state == STA_CLIMB) {
        Reset_On_Ground();
        m_velocity.x = 0.0f;
        m_velocity.y = 0.0f;
    }

    m_state = new_state;

    // # after new state is set
    if (m_state == STA_FLY) {
        // OLD Release_Item(1);
    }
}
