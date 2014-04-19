/***************************************************************************
 * lava.cpp  -  class for lava blocks
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

#include "lava.hpp"
#include "../level/level_player.hpp"
#include "../enemies/enemy.hpp"
#include "../core/xml_attributes.hpp"
#include "../core/game_core.hpp"
#include "../core/i18n.hpp"

using namespace SMC;

cLava::cLava(cSprite_Manager* p_sprite_manager)
	: cAnimated_Sprite(p_sprite_manager, "lava")
{
	Init();
}

cLava::cLava(XmlAttributes& attributes, cSprite_Manager* p_sprite_manager)
	: cAnimated_Sprite(p_sprite_manager, "lava")
{
	Init();
	Set_Pos(attributes.fetch<float>("posx", 0), attributes.fetch<float>("posy", 0), true);
}

cLava* cLava::Copy() const
{
	cLava* p_lava = new cLava(m_sprite_manager);
	p_lava->Set_Pos(m_start_pos_x, m_start_pos_y);
	return p_lava;
}

cLava::~cLava()
{
	//
}

void cLava::Init()
{
	m_type = TYPE_ACTIVE_SPRITE;
	m_sprite_array = ARRAY_LAVA;
	m_massive_type = MASS_MASSIVE;
	m_name = _("Lava");

	m_can_be_ground = true;
	Set_Scale_Directions(1, 1, 1, 1);

	Add_Image( pVideo->Get_Surface( "lava/red/red_1.png" ) );
	Add_Image( pVideo->Get_Surface( "lava/red/red_2.png" ) );
	Add_Image( pVideo->Get_Surface( "lava/red/red_3.png" ) );
	Add_Image( pVideo->Get_Surface( "lava/red/red_4.png" ) );
	Add_Image( pVideo->Get_Surface( "lava/red/red_5.png" ) );
	Add_Image( pVideo->Get_Surface( "lava/red/red_6.png" ) );

	Set_Animation(true);
	Set_Animation_Image_Range(0, 5);
	Set_Time_All(90, true);

	// set start image
	Reset_Animation();
	Set_Image_Num(m_anim_img_start, true, false);
}

void cLava::Update()
{
	Update_Animation();
}

void cLava::Draw(cSurface_Request* p_request /* = NULL */)
{
	if (!m_valid_draw)
		return;

	cAnimated_Sprite::Draw(p_request);
}

void cLava::Handle_Collision_Player(cObjectCollision* p_collision)
{
	// Lava collision means instant death.
	pLevel_Player->DownGrade(true);
}

void cLava::Handle_Collision_Enemy(cObjectCollision* p_collision)
{
	cEnemy* p_enemy = static_cast<cEnemy*>(p_collision->m_obj);

	// Lava collision means instant death.
	p_enemy->Set_Dead(true);
}

xmlpp::Element* cLava::Save_To_XML_Node(xmlpp::Element* p_element)
{
	xmlpp::Element* p_node = cAnimated_Sprite::Save_To_XML_Node(p_element);

	// No configuration currently

	return p_node;
}

std::string cLava::Get_XML_Type_Name()
{
	return "lava";
}
