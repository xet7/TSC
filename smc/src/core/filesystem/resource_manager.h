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
	bool Set_User_Directory( const std::string &dir );
	// Create the necessary folders in the user directory
	void Init_User_Directory( void );

	// user data directory
	std::string user_data_dir;

  // Return the path to the root directory containing graphics, music, etc.
  // This path is normally determined relatively to the `smc' executable,
  // but you can force a specific path at compile time by defining FIXED_DATA_DIR.
  boost::filesystem::path Get_Data_Directory( void );

  // Get pixmaps from the uncached game data directory.
  boost::filesystem::path Get_Game_Pixmaps_Dir();
  boost::filesystem::path Get_Game_Pixmap(std::string pixmap);
};

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

// Resource Manager
extern cResource_Manager *pResource_Manager;

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace SMC

#endif
