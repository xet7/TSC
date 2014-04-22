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

#ifndef SMC_BEETLE_BARRAGE_HPP
#define SMC_BEETLE_BARRAGE_HPP
#include "enemy.hpp"

namespace SMC {

	class cBeetleBarrage: public cEnemy
	{
	public:
		cBeetleBarrage(cSprite_Manager* p_sprite_manager);
		cBeetleBarrage(XmlAttributes& attributes, cSprite_Manager* p_sprite_manager);
		virtual ~cBeetleBarrage();

		virtual void DownGrade(bool force = false);
		void Set_Moving_State(Moving_state new_state);

		virtual Col_Valid_Type Validate_Collision(cSprite* p_obj);
		virtual void Handle_Collision_Player(cObjectCollision* p_collision);
		virtual void Handle_Collision_Enemy(cObjectCollision* p_collision);
		virtual void Handle_Collision_Massive(cObjectCollision* p_collision);

		virtual cBeetleBarrage* Copy() const;
		virtual void Update();
		virtual void Update_Dying();
		virtual bool Is_Update_Valid();

		virtual xmlpp::Element* Save_To_XML_Node(xmlpp::Element* p_element);

	protected:
		virtual std::string Get_XML_Type_Name();

	private:
		// Constructor common stuff
		void Init();

	};

}

#endif
