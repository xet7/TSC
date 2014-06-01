/***************************************************************************
 * crate.cpp
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

#include "crate.hpp"
#include "../core/xml_attributes.hpp"
#include "../core/sprite_manager.hpp"
#include "../core/i18n.hpp"
#include "../core/game_core.hpp"
#include "../audio/audio.hpp"
#include "../level/level_player.hpp"
#include "../enemies/enemy.hpp"

using namespace SMC;

cCrate::cCrate(cSprite_Manager* p_sprite_manager)
	: cAnimated_Sprite(p_sprite_manager, "crate")
{
	Init();
}

cCrate::cCrate(XmlAttributes& attributes, cSprite_Manager* p_sprite_manager)
	: cAnimated_Sprite(p_sprite_manager, "crate")
{
	Init();
	Set_Pos(attributes.fetch<float>("posx", 0), attributes.fetch<float>("posy", 0), true);
}

cCrate* cCrate::Copy() const
{
	cCrate* p_crate = new cCrate(m_sprite_manager);
	p_crate->Set_Pos(m_start_pos_x, m_start_pos_y);
	return p_crate;
}

cCrate::~cCrate()
{
	//
}

void cCrate::Init()
{
	m_type = TYPE_CRATE;
	m_sprite_array = ARRAY_ACTIVE;
	m_massive_type = MASS_MASSIVE;
	m_name = _("Crate");
	m_gravity_max = 22.0f;
	m_crate_state = CRATE_STAND;

	m_can_be_on_ground = true;
	m_can_be_ground = true;
	Set_Scale_Directions(1, 1, 1, 1);

	Add_Image(pVideo->Get_Surface("blocks/extra/box.png"));
	Set_Animation(false);
	Set_Image_Num(0, true, false);
}

void cCrate::Update()
{
	if (!m_valid_update || !Is_In_Range())
		return;
	if (m_crate_state == CRATE_DEAD)
		return;

	cAnimated_Sprite::Update();

	// Slow down if moving
	if (!Is_Float_Equal(m_velx, 0.0f)) {
		Add_Velocity_X(-m_velx * 0.2f);

		// Stop tolerance
		if (m_velx > -0.3f && m_velx < 0.3f)
			m_velx = 0.0f;
	}
}

std::string cCrate::Get_XML_Type_Name()
{
	return "crate";
}

xmlpp::Element* cCrate::Save_To_XML_Node(xmlpp::Element* p_element)
{
	xmlpp::Element* p_node = cAnimated_Sprite::Save_To_XML_Node(p_element);

	// No configuration currently

	return p_node;
}

void cCrate::Handle_Collision_Player(cObjectCollision* p_collision)
{
	if (m_crate_state == CRATE_DEAD)
		return;

	if (p_collision->m_direction == DIR_LEFT)
		m_velx = 20.0f;
	else if (p_collision->m_direction == DIR_RIGHT)
		m_velx = -20.0f;
	else if (p_collision->m_direction == DIR_BOTTOM) {
		// If the crate is moving down, it fell onto the player.
		if (m_vely > 0.5f)
			pLevel_Player->DownGrade_Player();
		return;
	}
	else // unsupported direction
		return;

	pAudio->Play_Sound("wood_1.ogg");
}

void cCrate::Handle_Collision_Enemy(cObjectCollision* p_collision)
{
	// When a crate falls onto an enemy, it gets killed.
	if (p_collision->m_direction == DIR_BOTTOM && m_vely > 0.5f) {
		cEnemy *p_enemy = static_cast<cEnemy*>(m_sprite_manager->Get_Pointer(p_collision->m_number));
		p_enemy->DownGrade(true);
	}
}

void cCrate::Handle_out_of_Level(ObjectDirection dir)
{
	// abyss
	if (dir == DIR_BOTTOM) {
		m_crate_state = CRATE_DEAD;
		m_massive_type = MASS_PASSIVE;
	}

	// Don’t move it outside right/left level edge
	if( dir == DIR_LEFT )
		Set_Pos_X(pActive_Camera->m_limit_rect.m_x - m_col_pos.m_x);
	else if( dir == DIR_RIGHT )
		Set_Pos_X(pActive_Camera->m_limit_rect.m_x + pActive_Camera->m_limit_rect.m_w - m_col_pos.m_x - m_col_rect.m_w - 0.01f);

}
