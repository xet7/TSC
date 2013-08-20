// -*- c++ -*-
/***************************************************************************
 * resource_manager.h
 *
 * Copyright (C) 2009 - 2011 Florian Richter
 ***************************************************************************/
/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SMC_RESOURCE_MANAGER_H
#define SMC_RESOURCE_MANAGER_H

#include "../../core/global_basic.h"
#include "../../core/global_game.h"

namespace SMC
{

/* *** *** *** *** *** cResource_Manager *** *** *** *** *** *** *** *** *** *** *** *** */

class cResource_Manager
{
public:
	cResource_Manager( void );
	~cResource_Manager( void );

	// Set the user data write directory
	bool Set_User_Directory( const boost::filesystem::path &dir );
	// Create the necessary folders in the user directory
	void Init_User_Directory( void );

	// user data directory
  boost::filesystem::path user_data_dir;

  // Return the path to the root directory containing graphics, music, etc.
  // This path is normally determined relatively to the `smc' executable,
  // but you can force a specific path at compile time by defining FIXED_DATA_DIR.
  boost::filesystem::path Get_Data_Directory( void );
  // The user’s data directory we can write to.
  boost::filesystem::path Get_User_Data_Directory();

  // Get the various uncached unwritable game directories.
  boost::filesystem::path Get_Game_Pixmaps_Directory();
  boost::filesystem::path Get_Game_Schema_Directory();
  boost::filesystem::path Get_Game_Level_Directory();
	boost::filesystem::path Get_Game_Translation_Directory();
	boost::filesystem::path Get_Game_Sounds_Directory();
	boost::filesystem::path Get_Game_Campaign_Directory();
	boost::filesystem::path Get_Game_Overworld_Directory();
	boost::filesystem::path Get_Game_Music_Directory();
	boost::filesystem::path Get_Game_Editor_Directory();
	boost::filesystem::path Get_Game_Scripting_Directory();
	boost::filesystem::path Get_Game_Icon_Directory();

	// CEGUI data paths
	boost::filesystem::path Get_Gui_Scheme_Directory();
	boost::filesystem::path Get_Gui_Imageset_Directory();
	boost::filesystem::path Get_Gui_Font_Directory();
	boost::filesystem::path Get_Gui_LookNFeel_Directory();
	boost::filesystem::path Get_Gui_Layout_Directory();

  // Get files from the various uncached unwritable game directories
  boost::filesystem::path Get_Game_Pixmap(std::string pixmap);
  boost::filesystem::path Get_Game_Schema(std::string schema);
  boost::filesystem::path Get_Game_Level(std::string level);
	boost::filesystem::path Get_Game_Translation(std::string transname);
	boost::filesystem::path Get_Game_Sound(std::string sound);
	boost::filesystem::path Get_Game_Campaign(std::string campaign);
	boost::filesystem::path Get_Game_Overworld(std::string overworld);
	boost::filesystem::path Get_Game_Music(std::string music);
	boost::filesystem::path Get_Game_Editor(std::string editor);
	boost::filesystem::path Get_Game_Scripting(std::string script);
	boost::filesystem::path Get_Game_Icon(std::string icon);

  // Get the various directories in the user’s data directory
  boost::filesystem::path Get_User_Level_Directory();
  boost::filesystem::path Get_User_Savegame_Directory();
  boost::filesystem::path Get_User_Screenshot_Directory();
  boost::filesystem::path Get_User_World_Directory();
  boost::filesystem::path Get_User_Campaign_Directory();
  boost::filesystem::path Get_User_Imgcache_Directory();

  // Get files from the various directories in the user’s data directory
  boost::filesystem::path Get_User_Level(std::string level);
};

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

// Resource Manager
extern cResource_Manager *pResource_Manager;

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace SMC

#endif
