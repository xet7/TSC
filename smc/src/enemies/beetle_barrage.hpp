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
		void Set_Active_Range(float range);
		float Get_Active_Range();
		void Set_Beetle_Interval(float time);
		float Get_Beelte_Interval();

		virtual Col_Valid_Type Validate_Collision(cSprite* p_obj);
		virtual void Handle_Collision_Player(cObjectCollision* p_collision);
		virtual void Handle_Collision_Enemy(cObjectCollision* p_collision);
		virtual void Handle_Collision_Massive(cObjectCollision* p_collision);

		virtual cBeetleBarrage* Copy() const;
		virtual void Draw(cSurface_Request* p_request = NULL);
		virtual void Update();
		virtual void Update_Dying();
		virtual bool Is_Update_Valid();

		virtual xmlpp::Element* Save_To_XML_Node(xmlpp::Element* p_element);

	protected:
		virtual std::string Get_XML_Type_Name();

	private:
		// Constructor common stuff
		void Init();

		/* Map m_active_range onto the current position for checking
		 * whether Maryo is there. For performance reasons we don’t
		 * return or reference it here, but set an instance member
		 * m_active_area.
		 * Depending on whether you want to calculate the circle for
		 * the current or for the starting point coords, pass the
		 * respective coordinates.
		 */
		void Calculate_Active_Area(const float& x, const float& y);

		// The area we react on Maryo.
		float m_active_range;
		GL_Circle m_active_area;
		float m_beetle_interval;
		float m_beetle_interval_counter;
		float m_spitting_beetles_counter;
		bool m_is_spitting_out_beetles;
	};

}

#endif
