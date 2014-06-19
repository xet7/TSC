/***************************************************************************
 * waypoint.cpp  -  waypoint class for the Overworld
 *
 * Copyright © 2003 - 2011 The SMC Contributors
 ***************************************************************************/
/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
 
#include "../overworld/world_waypoint.hpp"
#include "../overworld/overworld.hpp"
#include "../core/game_core.hpp"
#include "../core/framerate.hpp"
#include "../user/preferences.hpp"
#include "../video/renderer.hpp"
#include "../level/level.hpp"
#include "../core/math/utilities.hpp"
#include "../core/i18n.hpp"
#include "../video/gl_surface.hpp"
#include "../core/filesystem/filesystem.hpp"
#include "../core/filesystem/resource_manager.hpp"
#include "../core/xml_attributes.hpp"

namespace SMC
{

/* *** *** *** *** *** *** *** *** cWaypoint *** *** *** *** *** *** *** *** *** */

cWaypoint :: cWaypoint( cSprite_Manager *sprite_manager )
: cSprite( sprite_manager, "waypoint" )
{
	cWaypoint::Init();
}

cWaypoint :: cWaypoint( XmlAttributes &attributes, cSprite_Manager *sprite_manager )
: cSprite( sprite_manager, "waypoint" )
{
	cWaypoint::Init();

	// position
	Set_Pos(static_cast<float>(attributes.fetch<int>("x", 0)), static_cast<float>(attributes.fetch<int>("y", 0)), true);

	// image
	/*
	if (attributes.exists("image"))
		Set_Image(pVideo->Get_Surface(utf8_to_path(attributes["image"])), true);
	*/

	// type
	m_waypoint_type = static_cast<Waypoint_type>(attributes.fetch<int>("type", WAYPOINT_NORMAL));

	// destination
	// pre 0.99.6 : world
	if (attributes.exists("world"))
		Set_Destination(attributes["world"]);
	// pre 0.99.6 : level
	else if (attributes.exists("level"))
		Set_Destination(attributes["level"]);
	// default : destination
	else
		Set_Destination(attributes["destination"]);

	// access
	Set_Access(attributes.fetch<bool>("access", true), true);
}

cWaypoint :: ~cWaypoint( void )
{
	//
}

void cWaypoint :: Init( void )
{
	m_sprite_array = ARRAY_PASSIVE;
	m_type = TYPE_OW_WAYPOINT;
	m_massive_type = MASS_PASSIVE;
	m_pos_z = cSprite::m_pos_z_massive_start;
	m_camera_range = 0;
	
	m_waypoint_type = WAYPOINT_NORMAL;
	m_name = _("Waypoint");
	
	m_access = 0;
	m_access_default = 0;

	m_glim_color = Get_Random_Float( 0, 100 );
	m_glim_mod = 1;

	Set_Image( pVideo->Get_Surface( "world/waypoint/default_1.png" ) );
}

cWaypoint *cWaypoint :: Copy( void ) const
{
	cWaypoint *waypoint = new cWaypoint( m_sprite_manager );
	waypoint->Set_Pos( m_start_pos_x, m_start_pos_y, 1 );
	waypoint->Set_Image( m_start_image, 1 );
	waypoint->m_waypoint_type = m_waypoint_type;
	waypoint->Set_Destination( m_destination );
	waypoint->Set_Access( m_access_default, 1 );
	return waypoint;
}

std::string cWaypoint :: Get_XML_Type_Name()
{
	return int_to_string(m_waypoint_type);
}

xmlpp::Element* cWaypoint :: Save_To_XML_Node( xmlpp::Element* p_element)
{
	xmlpp::Element* p_node = cSprite::Save_To_XML_Node(p_element);

	// destination
	Add_Property(p_node, "destination", m_destination);
	// access
	Add_Property(p_node, "access", m_access_default);

	return p_node;
}

void cWaypoint :: Update( void )
{
	if( m_auto_destroy )
	{
		return;
	}

	cSprite::Update();

	if( m_glim_mod )
	{
		m_glim_color += pFramerate->m_speed_factor * 3.0f;
	}
	else
	{
		m_glim_color -= pFramerate->m_speed_factor * 3.0f;
	}

	if( m_glim_color > 120.0f )
	{
		m_glim_mod = 0;
	}
	else if( m_glim_color < 7.0f )
	{
		m_glim_mod = 1;
	}
}

void cWaypoint :: Draw( cSurface_Request *request /* = NULL  */ )
{
	if( m_auto_destroy )
	{
		return;
	}

	// draw waypoint
	if( ( m_access && ( m_waypoint_type == 1 || m_waypoint_type == 2 ) ) || pOverworld_Manager->m_debug_mode || editor_world_enabled )
	{
		bool create_request = 0;

		if( !request )
		{
			create_request = 1;
			// create request
			request = new cSurface_Request();
		}

		// draw
		cSprite::Draw( request );

		// default color
		if( !pOverworld_Manager->m_debug_mode )
		{
			request->m_color = Color( static_cast<Uint8>(255), 100 + static_cast<Uint8>(m_glim_color), 10 );
		}
		// change to debug color
		else
		{
			if( m_access )
			{
				request->m_color = Color( static_cast<Uint8>(255), 100 + static_cast<Uint8>(m_glim_color), 200 );
			}
			else
			{
				request->m_color =  Color( static_cast<Uint8>(20), 100 + static_cast<Uint8>(m_glim_color), 10 );
			}
		}

		if( create_request )
		{
			// add request
			pRenderer->Add( request );
		}
	}
}

void cWaypoint :: Set_Access( bool enabled, bool new_start_access /* = 0 */ )
{
	m_access = enabled;
	
	if( new_start_access )
	{
		m_access_default = m_access;
	}
}

void cWaypoint :: Set_Destination( std::string level_or_worldname )
{
	m_destination = level_or_worldname;
}

std::string cWaypoint :: Get_Destination() const
{
	return m_destination;
}

boost::filesystem::path cWaypoint :: Get_Destination_Path()
{
	switch(m_waypoint_type) {
	case WAYPOINT_NORMAL:
		return pLevel_Manager->Get_Path(m_destination);
	case WAYPOINT_WORLD_LINK:
		return pResource_Manager->Get_Game_Overworld(m_destination);
	default:
		// FIXME: Throw an exception
		std::cerr << "Error: Undefined waypoint type" << m_waypoint_type << std::endl;
		return boost::filesystem::path();
	}
}

void cWaypoint :: Editor_Activate( void )
{
	// get window manager
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

	// Type
	CEGUI::Combobox *combobox = static_cast<CEGUI::Combobox *>(wmgr.createWindow( "TaharezLook/Combobox", "waypoint_type" ));
	Editor_Add( UTF8_("Type"), UTF8_("Destination type"), combobox, 120, 80 );

	combobox->addItem( new CEGUI::ListboxTextItem( UTF8_("Level") ) );
	combobox->addItem( new CEGUI::ListboxTextItem( UTF8_("World") ) );

	if( m_waypoint_type == WAYPOINT_NORMAL )
	{
		combobox->setText( UTF8_("Level") );
	}
	else
	{
		combobox->setText( UTF8_("World") );
	}

	combobox->subscribeEvent( CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber( &cWaypoint::Editor_Type_Select, this ) );


	// destination
	CEGUI::Editbox *editbox = static_cast<CEGUI::Editbox *>(wmgr.createWindow( "TaharezLook/Editbox", "waypoint_destination" ));
	Editor_Add( UTF8_("Destination"), UTF8_("Destination level or world"), editbox, 150 );

	editbox->setText( Get_Destination() );
	editbox->subscribeEvent( CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber( &cWaypoint::Editor_Destination_Text_Changed, this ) );

	// Access
	combobox = static_cast<CEGUI::Combobox *>(wmgr.createWindow( "TaharezLook/Combobox", "waypoint_access" ));
	Editor_Add( UTF8_("Default Access"), UTF8_("Enable if the Waypoint should be always accessible."), combobox, 120, 80 );

	combobox->addItem( new CEGUI::ListboxTextItem( UTF8_("Enabled") ) );
	combobox->addItem( new CEGUI::ListboxTextItem( UTF8_("Disabled") ) );

	if( m_access_default )
	{
		combobox->setText( UTF8_("Enabled") );
	}
	else
	{
		combobox->setText( UTF8_("Disabled") );
	}

	combobox->subscribeEvent( CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber( &cWaypoint::Editor_Access_Select, this ) );

	// init
	Editor_Init();
}

bool cWaypoint :: Editor_Type_Select( const CEGUI::EventArgs &event )
{
	const CEGUI::WindowEventArgs &windowEventArgs = static_cast<const CEGUI::WindowEventArgs&>( event );
	CEGUI::ListboxItem *item = static_cast<CEGUI::Combobox *>( windowEventArgs.window )->getSelectedItem();

	if( item->getText().compare( UTF8_("Level") ) == 0 )
	{
		m_waypoint_type = WAYPOINT_NORMAL;
	}
	else
	{
		m_waypoint_type = WAYPOINT_WORLD_LINK;
	}

	return 1;

}
bool cWaypoint :: Editor_Destination_Text_Changed( const CEGUI::EventArgs &event )
{
	const CEGUI::WindowEventArgs &windowEventArgs = static_cast<const CEGUI::WindowEventArgs&>( event );
	std::string str_text = static_cast<CEGUI::Editbox *>( windowEventArgs.window )->getText().c_str();

	Set_Destination( str_text );

	return 1;
}

bool cWaypoint :: Editor_Access_Select( const CEGUI::EventArgs &event )
{
	const CEGUI::WindowEventArgs &windowEventArgs = static_cast<const CEGUI::WindowEventArgs&>( event );
	CEGUI::ListboxItem *item = static_cast<CEGUI::Combobox *>( windowEventArgs.window )->getSelectedItem();

	if( item->getText().compare( UTF8_("Enabled") ) == 0 )
	{
		Set_Access( 1, 1 );
	}
	else
	{
		Set_Access( 0, 1 );
	}

	return 1;
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace SMC
