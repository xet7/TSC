//#ifdef STOP
/***************************************************************************
 * larry.cpp - Run or die.
 *
 * Copyright © 2014 The TSC Contributors
 ***************************************************************************/
/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "doom_larry.hpp"
#include "../core/xml_attributes.hpp"
#include "../core/game_core.hpp"
#include "../core/sprite_manager.hpp"
#include "../core/i18n.hpp"
#include "../level/level_player.hpp"
#include "../level/level.hpp"
#include "../video/animation.hpp"
#include "../objects/box.hpp"

using namespace TSC;

cDoomLarry::cDoomLarry(cSprite_Manager* p_sprite_manager)
    : cLarry(p_sprite_manager)
{
    Init();
}

cDoomLarry::cDoomLarry(XmlAttributes& attributes, cSprite_Manager* p_sprite_manager)
    : cLarry(p_sprite_manager)
{
    Init();

    Set_Pos(attributes.fetch<float>("posx", 0), attributes.fetch<float>("posy", 0), true);
    Set_Direction(Get_Direction_Id(attributes.fetch("direction", Get_Direction_Name(m_start_direction))));
}

cDoomLarry::~cDoomLarry()
{
    //
}

void cDoomLarry::Init()
{
    //The Doom_Larry constructor will have called the Larry constructor, creating an image set full of grey Larry images that we do not want
    //Remove them so that they can be replaced with Doom Larry images.  If Init() is called again, this clear will also be needed again.
    Clear_Images(true, true);

    m_type = TYPE_DOOM_LARRY;
    m_name = "Doom Larry";
    m_pos_z = 0.09f;
    m_gravity_max = 29.0f;

    m_kill_points = 300;
    m_fire_resistant = false;
    m_ice_resistance = 1.0f;
    m_can_be_hit_from_shell = true;
    m_explosion_counter = 0.0f;
    m_kill_sound = "enemy/larry/red/die.ogg";

    Add_Image_Set("walk", "enemy/larry/red/walk.imgset");
    Add_Image_Set("walk_turn", "enemy/larry/red/walk_turn.imgset", 0, &m_walk_turn_start, &m_walk_turn_end);
    Add_Image_Set("run", "enemy/larry/red/run.imgset");
    Add_Image_Set("run_turn", "enemy/larry/red/run_turn.imgset", 0, &m_run_turn_start, &m_run_turn_end);
    Add_Image_Set("action", "enemy/larry/red/action.imgset", 0, &m_action_start, &m_action_end);

    m_state = STA_STAY;         //This triggers logic in method Set_Moving_State to reset the start image after having called method Clear_Images above
    Set_Moving_State(STA_WALK);
    Set_Direction(DIR_RIGHT);
}

cDoomLarry* cDoomLarry::Copy() const
{
    cDoomLarry* p_dl = new cDoomLarry(m_sprite_manager);
    p_dl->Set_Pos(m_start_pos_x, m_start_pos_y);
    p_dl->Set_Direction(m_start_direction);
    return p_dl;
}

std::string cDoomLarry::Get_XML_Type_Name()
{
    return "doom_larry";
}

#ifdef JUNK
xmlpp::Element* cLarry::Save_To_XML_Node(xmlpp::Element* p_element)
{
    xmlpp::Element* p_node = cEnemy::Save_To_XML_Node(p_element);

    Add_Property(p_node, "direction", Get_Direction_Name(m_start_direction));

    return p_node;
}
#endif

#ifdef JUNK
void cLarry::DownGrade(bool force /* = false */)
{
    if (m_state == STA_RUN || force) {
        Set_Dead(true);
        m_massive_type = MASS_PASSIVE;
        m_velx = 0.0f;
        m_vely = 0.0f;

        pAudio->Play_Sound(m_kill_sound);
        Explosion_Animation();
    }
    else if (m_state == STA_WALK)
        Fuse();
}
#endif

void cDoomLarry::Update()
{
    cEnemy::Update();
    if (!m_valid_update || !Is_In_Range())
        return;

    Update_Animation();
    Update_Velocity();

    if (m_state == STA_RUN) {
        m_explosion_counter += pFramerate->m_speed_factor;

        if (m_explosion_counter > 200.0f)
            DownGrade(true);
    }

    // If currently turning ’round
    if ((m_curr_img >= m_walk_turn_start && m_curr_img <= m_walk_turn_end) ||
        (m_curr_img >= m_run_turn_start && m_curr_img <= m_run_turn_end)) {
        m_anim_counter += pFramerate->m_elapsed_ticks;

        if ((m_state == STA_WALK && m_anim_counter >= 600) || /* normal walk */
                (m_state == STA_RUN && m_anim_counter >= 100)) { /* fusing */

            if (m_state == STA_WALK) {
                Set_Image_Set("walk");
                m_velx_max = 1.5f;
            } else if (m_state == STA_RUN) {
                Set_Image_Set("run");
                m_velx_max = 3.0f;
            }
            else {
                throw (TSCError("Invalid larry walking state!"));
            }

            Update_Rotation_Hor();
        }
    }
    else if (m_curr_img >= m_action_start && m_curr_img <= m_action_end) { // if currently activating
        m_anim_counter += pFramerate->m_elapsed_ticks;

        // back to normal animation
        if (m_anim_counter >= 600) {
            Set_Image_Set("run");
            m_velx_max = 3.0f;

            Update_Rotation_Hor();
        }
    }
}

#ifdef JUNK
void cLarry::Update_Normal_Dying()
{
    // Hide larry behind the explosion clouds
    if (m_dying_counter > 12.0f) {
        m_valid_draw = false;
    }
    // After a little more time, kill everything in range and completely
    // remove larry from the game.
    if (m_dying_counter > 24.0f) {
        Set_Active(false);
        Kill_Objects_in_Explosion_Range();
    }
}
#endif

#ifdef JUNK
void cLarry::Set_Direction(const ObjectDirection dir, bool initial /* = true */)
{
    if (m_start_direction == dir)
        return;

    cEnemy::Set_Direction(dir, initial);
    Update_Rotation_Hor(true);
}
#endif

#ifdef JUNK
Col_Valid_Type cLarry::Validate_Collision(cSprite* p_sprite)
{
    Col_Valid_Type basic_valid = Validate_Collision_Ghost(p_sprite);
    if (basic_valid != COL_VTYPE_NOT_POSSIBLE)
        return basic_valid;

    if (p_sprite->m_massive_type == MASS_MASSIVE) {
        switch (p_sprite->m_type) {
        case TYPE_FLYON: // fallthrough
        case TYPE_ROKKO:
        case TYPE_GEE:
        case TYPE_SPIKA:
        case TYPE_STATIC_ENEMY:
        case TYPE_TURTLE_BOSS:
            return COL_VTYPE_NOT_VALID;
        default:
            return COL_VTYPE_BLOCKING;
        }
    }
    else if (p_sprite->m_massive_type == MASS_HALFMASSIVE) {
        if (m_vely >= 0.0f && Is_On_Top(p_sprite))
            return COL_VTYPE_BLOCKING;
    }
    else if (p_sprite->m_massive_type == MASS_PASSIVE) {
        if (p_sprite->m_type == TYPE_ENEMY_STOPPER)
            return COL_VTYPE_BLOCKING;
    }

    return COL_VTYPE_NOT_VALID;
}
#endif

#ifdef JUNK
void cLarry::Handle_Collision_Massive(cObjectCollision* p_collision)
{
    cEnemy::Handle_Collision_Massive(p_collision);

    //Send_Collision(p_collision);

    // get colliding object
    cSprite* p_collidor = m_sprite_manager->Get_Pointer(p_collision->m_number);

    if (p_collidor->m_type == TYPE_BALL) {
        cBall* p_ball = static_cast<cBall*>(p_collidor);

        // Immediately explode if hit by a fireball
        if (p_ball->m_ball_type == FIREBALL_DEFAULT)
            DownGrade(true);
    }

    if (p_collision->m_direction == DIR_RIGHT || p_collision->m_direction == DIR_LEFT)
        Turn_Around(p_collision->m_direction);
}
#endif

#ifdef JUNK
void cLarry::Handle_Collision_Player(cObjectCollision* p_collision)
{
    if (p_collision->m_direction == DIR_UNDEFINED)
        return;

    if (p_collision->m_direction == DIR_TOP) {
        DownGrade();
        pLevel_Player->Action_Jump(true);
    }
    else
        pLevel_Player->DownGrade_Player();

    if (p_collision->m_direction == DIR_LEFT || p_collision->m_direction == DIR_RIGHT)
        Turn_Around();
}
#endif

#ifdef JUNK
void cLarry::Handle_Collision_Enemy(cObjectCollision* p_collision)
{
    if (p_collision->m_direction == DIR_LEFT || p_collision->m_direction == DIR_RIGHT)
        Turn_Around();
}
#endif

#ifdef JUNK
void cLarry::Handle_Ball_Hit(const cBall& ball, const cObjectCollision* p_collision)
{
    if (ball.m_ball_type != FIREBALL_DEFAULT)
        return;

    // Do NOT set larry to inactive here! This must be done in Update_Normal_Dying()
    // in order to ensure he disappears in the smoke of the explosion and not beforehand.
    Ball_Destroy_Animation(ball);
    DownGrade(true);
    pLevel_Player->Add_Kill_Multiplier();
}
#endif

#ifdef JUNK
void cLarry::Turn_Around(ObjectDirection col_dir /* = DIR_UNDEFINED */)
{
    cEnemy::Turn_Around(col_dir);

    if (m_state == STA_WALK)
        Set_Image_Set("walk_turn");
    else
        Set_Image_Set("run_turn");

    // Stop walking while turning (reset to normal in Update())
    m_velx = 0.0f;
    m_velx_max = 0.0f;
    Update_Rotation_Hor();
}
#endif

void cDoomLarry::Set_Moving_State(Moving_state new_state)
{
    if (new_state == m_state)
        return;

    m_state = new_state;

    if (m_state == STA_WALK) {
        Set_Image_Set("walk", true);

        m_velx_gain = 0.3f;
        m_velx_max = 1.5f;
    }
    else if (m_state == STA_RUN) {
        Set_Image_Set("run");
    }

    Reset_Animation();
    Update_Rotation_Hor(); // In case of change in turning animation
}

#ifdef JUNK
void cLarry::Fuse()
{
    Set_Moving_State(STA_RUN);

    // Stop walking for a moment (reset to normal in Update())
    Set_Image_Set("action");
    m_velx = 0.0f;
    m_velx_max = 0.0f;
    Update_Rotation_Hor();
}
#endif

void cDoomLarry::Kill_Objects_in_Explosion_Range()
{
    GL_Circle explosion_radius(m_pos_x + m_rect.m_w / 2.0,
                               m_pos_y + m_rect.m_h / 2.0,
                               200.0f);

    // Find all objects we can destroy (note that if another
    // Larry is inside this radius, he will explode likewise
    // and check himself for further destructions in his radius)
    cSprite_List objects;
    pActive_Level->m_sprite_manager->Get_Colliding_Objects(objects, explosion_radius, true, this);

    // DESTROY ’EM ALL.
    // Alex WILL be killed instantly.
    cSprite_List::iterator iter;
    for (iter=objects.begin(); iter != objects.end(); iter++) {
        cSprite* p_obj = *iter;
        cEnemy* p_enemy = NULL;
        cBaseBox* p_box = NULL;

        if (p_obj->m_type == TYPE_PLAYER) // This means p_obj == pLevel_Player
            pLevel_Player->DownGrade_Player(true, true);
        else if ((p_enemy = dynamic_cast<cEnemy*>(p_obj)))
            p_enemy->DownGrade(true);
        else if ((p_box = dynamic_cast<cBaseBox*>(p_obj)))
            p_box->Activate();
    }
}

#ifdef JUNK
void cLarry::Explosion_Animation()
{
    cParticle_Emitter* p_em = new cParticle_Emitter(m_sprite_manager);
    p_em->Set_Emitter_Rect(m_col_rect);
    p_em->Set_Quota(10);
    p_em->Set_Pos_Z(cSprite::m_pos_z_front_passive_start + 0.01f);
    p_em->Set_Image(pVideo->Get_Package_Surface("animation/particles/smoke.png"));
    p_em->Set_Time_to_Live(3.5f);
    p_em->Set_Scale(1.5f);
    p_em->Set_Emitter_Time_to_Live(2.0f);
    pActive_Animation_Manager->Add(p_em);

    p_em = new cParticle_Emitter(m_sprite_manager);
    p_em->Set_Emitter_Rect(m_col_rect);
    p_em->Set_Quota(5);
    p_em->Set_Pos_Z(cSprite::m_pos_z_front_passive_start + 0.01f);
    p_em->Set_Image(pVideo->Get_Package_Surface("animation/particles/smoke_grey_big.png"));
    p_em->Set_Time_to_Live(5.0f);
    p_em->Set_Scale(1.5f);
    p_em->Set_Emitter_Time_to_Live(2.0f);
    pActive_Animation_Manager->Add(p_em);

    p_em = new cParticle_Emitter(m_sprite_manager);
    p_em->Set_Emitter_Rect(m_col_rect);
    p_em->Set_Quota(5);
    p_em->Set_Pos_Z(cSprite::m_pos_z_front_passive_start + 0.02f);
    p_em->Set_Image(pVideo->Get_Package_Surface("animation/particles/cloud.png"));
    p_em->Set_Time_to_Live(7.0f);
    p_em->Set_Scale(1.0f);
    p_em->Set_Emitter_Time_to_Live(2.0f);
    pActive_Animation_Manager->Add(p_em);
}
#endif

#ifdef JUNK
void cDoomLarry::Editor_Activate()
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();

    // direction
    CEGUI::Combobox* p_combobox = static_cast<CEGUI::Combobox*>(wmgr.createWindow("TaharezLook/Combobox", "editor_larry_direction"));
    Editor_Add(UTF8_("Direction"), UTF8_("Starting direction."), p_combobox, 100, 75);

    p_combobox->addItem(new CEGUI::ListboxTextItem("left"));
    p_combobox->addItem(new CEGUI::ListboxTextItem("right"));
    p_combobox->setText(Get_Direction_Name(m_start_direction));
    p_combobox->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&cDoomLarry::On_Editor_Direction_Select, this));

    Editor_Init();
}
#endif

#ifdef JUNK
bool cLarry::On_Editor_Direction_Select(const CEGUI::EventArgs& event)
{
    const CEGUI::WindowEventArgs& args = static_cast<const CEGUI::WindowEventArgs&>(event);
    CEGUI::ListboxItem* p_item = static_cast<CEGUI::Combobox*>(args.window)->getSelectedItem();

    Set_Direction(Get_Direction_Id(p_item->getText().c_str()));

    return true;
}
#endif

//#endif
