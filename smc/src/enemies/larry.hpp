/***************************************************************************
 * larry.hpp - The walking bomb.
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

#ifndef SMC_LARRY_HPP
#define SMC_LARRY_HPP
#include "enemy.hpp"

namespace SMC {

	class cLarry: public cEnemy
	{
	public:
		cLarry(cSprite_Manager* p_sprite_manager);
		cLarry(XmlAttributes& attributes, cSprite_Manager* p_sprite_manager);
		virtual ~cLarry();

		virtual cLarry* Copy() const;
		virtual void Update();
		virtual void Update_Normal_Dying();
		virtual void Handle_Collision_Massive(cObjectCollision* p_collision);
		virtual void Handle_Collision_Player(cObjectCollision* p_collision);
		virtual void Handle_Collision_Enemy(cObjectCollision* p_collision);

		void Fuse();
		virtual void DownGrade(bool force = false);
		virtual void Set_Direction(const ObjectDirection dir, bool initial = true);
		virtual void Turn_Around(ObjectDirection col_dir = DIR_UNDEFINED);
		virtual void Set_Moving_State(Moving_state new_state);

		virtual xmlpp::Element* Save_To_XML_Node(xmlpp::Element* p_element);

	protected:
		virtual std::string Get_XML_Type_Name();

	private:
		void Init();
		void Kill_Objects_in_Explosion_Range();
		void Explosion_Animation();

		float m_explosion_counter;
	};

}

#endif
