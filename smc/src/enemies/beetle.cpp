/***************************************************************************
 * pip.hpp
 *
 * Copyright © 2014 The SMC Contributors
 ***************************************************************************/
/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "beetle.hpp"
#include "../core/errors.hpp"
#include "../core/xml_attributes.hpp"
#include "../core/game_core.hpp"
#include "../core/math/circle.hpp"
#include "../level/level_player.hpp"
#include "../gui/hud.hpp"

using namespace SMC;

cBeetle::cBeetle(cSprite_Manager* p_sprite_manager)
	: cEnemy(p_sprite_manager)
{
	Init();
}

cBeetle::cBeetle(XmlAttributes& attributes, cSprite_Manager* p_sprite_manager)
	: cEnemy(p_sprite_manager)
{
	Init();

	// position
	Set_Pos(attributes.fetch<float>("posx", 0), attributes.fetch<float>("posy", 0), true);
}

cBeetle::~cBeetle()
{
	//
}

void cBeetle::Init()
{
	m_type = TYPE_BEETLE;
	m_pos_z = 0.093f;
	m_gravity_max = 0.0f;
	m_editor_pos_z = 0.089f;
	m_name = "Beetle";
	m_velx = -2.5;
	m_rest_living_time = Get_Random_Float(150.0f, 250.0f);

	// TODO: Randomize color
	Add_Image(pVideo->Get_Surface(utf8_to_path("enemy/beetle/blue/left_1.png")));
	Add_Image(pVideo->Get_Surface(utf8_to_path("enemy/beetle/blue/left_2.png")));
	Add_Image(pVideo->Get_Surface(utf8_to_path("enemy/beetle/blue/left_3.png")));

	m_state = STA_FLY;
	Set_Direction(DIR_LEFT);

	// TODO: Own die sound
	m_kill_sound = "enemy/gee/die.ogg";
	m_kill_points = 100;

	Set_Animation(true);
	Set_Animation_Image_Range(0, 2);
	Set_Time_All(140, true);
	Reset_Animation();
	Set_Image_Num(0, true);
}

cBeetle* cBeetle::Copy() const
{
	cBeetle* p_beetle = new cBeetle(m_sprite_manager);
	p_beetle->Set_Pos(m_start_pos_x, m_start_pos_y);
	p_beetle->Set_Direction(m_start_direction);
	return p_beetle;
}

std::string cBeetle::Get_XML_Type_Name()
{
	return "beetle";
}

xmlpp::Element* cBeetle::Save_To_XML_Node(xmlpp::Element* p_element)
{
	xmlpp::Element* p_node = cEnemy::Save_To_XML_Node(p_element);

	Add_Property(p_node, "direction", Get_Direction_Name(m_start_direction));

	return p_node;
}

void cBeetle::DownGrade(bool force /* = false */)
{
	if (force) // Falling/lava death
		Set_Rotation_Z(180.0f);

	Set_Dead(true);
	m_massive_type = MASS_PASSIVE;
	m_velx = 0.0f;
	m_vely = 0.0f;
}

void cBeetle::Update_Dying()
{
	m_counter += pFramerate->m_speed_factor;

	// Forced death
	if (Is_Float_Equal(m_rot_z, 180.0f)) {
		// A little bit upwards first
		if (m_counter < 5.0f)
			Move(0.0f, 5.0f);
		// if not below the ground: fall
		else if (m_col_rect.m_y < pActive_Camera->m_limit_rect.m_y)
			Move(0.0f, 20.0f);
		// if below disable
		else {
			m_rot_z = 0.0f;
			Set_Active(false);
		}
	}
	// Normal death
	else {
		float speed = pFramerate->m_speed_factor * 0.05f;
		Add_Scale_X(-speed * 0.5f);
		Add_Scale_Y(-speed);

		if (m_scale_y < 0.01f) {
			Set_Scale(1.0f);
			Set_Active(false);
		}
	}
}

void cBeetle::Update()
{
	cEnemy::Update();

	if (!m_valid_update || !Is_In_Range())
		return;

	// When living time is up, die.
	m_rest_living_time -= pFramerate->m_speed_factor;
	if (m_rest_living_time <= 0) {
		m_rest_living_time = 0;
		DownGrade(true);
	}

	// This enemy is immune to both gravity and air resistance.
	// We don’t update the velocity or gravity therefore.
	Update_Animation();
}

void cBeetle::Draw(cSurface_Request* p_request /* = NULL */)
{
	if (!m_valid_draw)
		return;

	cEnemy::Draw(p_request);
}

bool cBeetle::Is_Update_Valid()
{
	if (m_dead || m_freeze_counter)
		return false;
	else
		return true;
}

Col_Valid_Type cBeetle::Validate_Collision(cSprite* p_obj)
{
	if (p_obj->m_massive_type == MASS_MASSIVE) {
		switch(p_obj->m_type) {
		case TYPE_PLAYER:
		case TYPE_BALL: // fallthru
			return COL_VTYPE_INTERNAL;
		default:
			break;
		}
		return COL_VTYPE_NOT_VALID;
	}
	else
		return COL_VTYPE_NOT_VALID;
}

void cBeetle::Handle_Collision_Player(cObjectCollision* p_collision)
{
	// invalid
	if (p_collision->m_direction == DIR_UNDEFINED)
		return;

	// We will die only when hit from the top
	if (p_collision->m_direction == DIR_TOP && pLevel_Player->m_state != STA_FLY) {
		pAudio->Play_Sound(m_kill_sound);
		DownGrade();
		pLevel_Player->Action_Jump(true);
		pHud_Points->Add_Points(m_kill_points, m_pos_x, m_pos_y - 5.0f, "", static_cast<Uint8>(255), 1 );
		pLevel_Player->Add_Kill_Multiplier();
	}
	else if (!pLevel_Player->m_invincible) {
		if (p_collision->m_direction == DIR_LEFT) {
			pLevel_Player->m_velx = -7.0f;
			Turn_Around();
		}
		else if (p_collision->m_direction == DIR_RIGHT) {
			pLevel_Player->m_velx = 7.0f;
			Turn_Around();
		}

		pLevel_Player->DownGrade_Player();
	}
}

void cBeetle::Handle_Collision_Massive(cObjectCollision* p_collision)
{
	Send_Collision(p_collision);
}

float cBeetle::Get_Rest_Living_Time()
{
	return m_rest_living_time;
}

void cBeetle::Set_Rest_Living_Time(float time)
{
	m_rest_living_time = time;
}
