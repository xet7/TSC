/***************************************************************************
 * thromp.h
 *
 * Copyright (C) 2006 - 2011 Florian Richter
 ***************************************************************************/
/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SMC_THROMP_H
#define SMC_THROMP_H

#include "../enemies/enemy.h"
#include "../scripting/objects/enemies/mrb_thromp.h"

namespace SMC
{

/* *** *** *** *** *** cThromp *** *** *** *** *** *** *** *** *** *** *** *** */
/* The Falling Stones
*/
class cThromp : public cEnemy
{
public:
	// constructor
	cThromp( cSprite_Manager *sprite_manager );
	// create from stream
	cThromp( XmlAttributes &attributes, cSprite_Manager *sprite_manager );
	// destructor
	virtual ~cThromp( void );

	// init defaults
	void Init( void );
	// copy
	virtual cThromp *Copy( void ) const;

	// Create the MRuby object for this
	virtual mrb_value Create_MRuby_Object(mrb_state* p_state)
	{
		return mrb_obj_value(Data_Wrap_Struct(p_state, Scripting::p_rcThromp, &Scripting::rtSMC_Scriptable, this));
	}


	// load from savegame
	virtual void Load_From_Savegame( cSave_Level_Object *save_object );
	// save to savegame
	virtual cSave_Level_Object *Save_To_Savegame( void );

	// Set the image directory. `dir' must be relative to the pixmaps/ directory.
	void Set_Image_Dir( boost::filesystem::path dir );
	// Set Direction
	virtual void Set_Direction( const ObjectDirection dir, bool initial = true );
	// Set Max Distance
	void Set_Max_Distance( float nmax_distance );
	// Set the Speed
	void Set_Speed( float val );

	// Move to destination direction
	void Activate( void );
	// Move back to the original position
	bool Move_Back( void );

	/* downgrade state ( if already the weakest state it dies )
	 * force : usually dies or a complete downgrade
	*/
	virtual void DownGrade( bool force = 0 );
	// dying animation update
	virtual void Update_Dying( void );

	// update
	virtual void Update( void );
	// draw
	virtual void Draw( cSurface_Request *request = NULL );

	// update images
	void Update_Images( void );
	// update destination velocity from the speed
	void Update_Dest_Vel( void );
	// update the distance rect
	void Update_Distance_Rect( void );
	// Get the final distance rect
	GL_rect Get_Final_Distance_Rect( void ) const;

	// Generates Smoke Particles
	void Generate_Smoke( unsigned int amount = 20 ) const;

	// if update is valid for the current state
	virtual bool Is_Update_Valid( void );
	// if draw is valid for the current state and position
	virtual bool Is_Draw_Valid( void );

	/* Validate the given collision object
	 * returns 0 if not valid
	 * returns 1 if an internal collision with this object is valid
	 * returns 2 if the given object collides with this object (blocking)
	*/
	virtual Col_Valid_Type Validate_Collision( cSprite *obj );
	// collision from player
	virtual void Handle_Collision_Player( cObjectCollision *collision );
	// collision from an enemy
	virtual void Handle_Collision_Enemy( cObjectCollision *collision );
	// collision with massive
	virtual void Handle_Collision_Massive( cObjectCollision *collision );
	// handle moved out of Level event
	virtual void Handle_out_of_Level( ObjectDirection dir );

	// editor activation
	virtual void Editor_Activate( void );
	// editor direction option selected event
	bool Editor_Direction_Select( const CEGUI::EventArgs &event );
	// editor image dir text changed event
	bool Editor_Image_Dir_Text_Changed( const CEGUI::EventArgs &event );
	// editor max distance text changed event
	bool Editor_Max_Distance_Text_Changed( const CEGUI::EventArgs &event );
	// editor speed text changed event
	bool Editor_Speed_Text_Changed( const CEGUI::EventArgs &event );

	// image directory
	boost::filesystem::path m_img_dir;
	// speed
	float m_speed;
	// destination direction velocity
	float m_dest_velx, m_dest_vely;
	// maximum distance from the startposition
	float m_max_distance;

	// moving back to the original position
	bool m_move_back;
	// distance rect to end position
	GL_rect m_distance_rect;

	// Save to XML node
	virtual xmlpp::Element* Save_To_XML_Node(xmlpp::Element* p_element);

protected:
	
	virtual std::string Get_XML_Type_Name();

private:
	// Create the Name from the current settings
	void Create_Name( void );
};

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace SMC

#endif
