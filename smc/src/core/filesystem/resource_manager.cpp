/***************************************************************************
 * resource_manager.cpp  -  Resource Manager
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

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux)
#include <limits.h>
#endif
#include <boost/filesystem/path.hpp>

#include "resource_manager.h"
#include "filesystem.h"
#include "../property_helper.h"

using namespace boost::filesystem;

namespace SMC
{

/* *** *** *** *** *** *** cResource_Manager *** *** *** *** *** *** *** *** *** *** *** */

cResource_Manager :: cResource_Manager( void )
{
	user_data_dir = "";
}

cResource_Manager :: ~cResource_Manager( void )
{

}

void cResource_Manager :: Init_User_Directory( void )
{
	// Create user directory
	if( !Dir_Exists( user_data_dir ) )
	{
		// first run if not available
		Create_Directory( user_data_dir );
	}
	// Create savegame directory
	if( !Dir_Exists( user_data_dir + USER_SAVEGAME_DIR ) )
	{
		Create_Directory( user_data_dir + USER_SAVEGAME_DIR );
	}
	// Create screenshot directory
	if( !Dir_Exists( user_data_dir + USER_SCREENSHOT_DIR ) )
	{
		Create_Directory( user_data_dir + USER_SCREENSHOT_DIR );
	}
	// Create level directory
	if( !Dir_Exists( user_data_dir + USER_LEVEL_DIR ) )
	{
		Create_Directory( user_data_dir + USER_LEVEL_DIR );
	}
	// Create world directory
	if( !Dir_Exists( user_data_dir + USER_WORLD_DIR ) )
	{
		Create_Directory( user_data_dir + USER_WORLD_DIR );
	}
	// Create campaign directory
	if( !Dir_Exists( user_data_dir + USER_CAMPAIGN_DIR ) )
	{
		Create_Directory( user_data_dir + USER_CAMPAIGN_DIR );
	}
	// Create cache directory
	if( !Dir_Exists( user_data_dir + USER_IMGCACHE_DIR ) )
	{
		Create_Directory( user_data_dir + USER_IMGCACHE_DIR );
	}
}

bool cResource_Manager :: Set_User_Directory( const std::string &dir )
{
	user_data_dir = dir;

	return 1;
}

path cResource_Manager :: Get_Data_Directory( void )
{
#ifdef FIXED_DATA_DIR
  return utf8_to_path(FIXED_DATA_DIR);
#else
#ifdef _WIN32
  wchar_t path_data[MAX_PATH + 1];

  if (GetModuleFileNameW(NULL, path_data, MAX_PATH) == 0)
    throw "Failed to retrieve the executable's path from the Win32API!";

  std::string utf8_path = ucs2_to_utf8(path_data);
  Convert_Path_Separators(utf8_path);

  return utf8_to_path(utf8_path);
#elif __linux
  char path_data[PATH_MAX];

  if (readlink("/proc/self/exe", path_data, PATH_MAX) < 0)
    throw "Failed to retrieve the executable's path from /proc/self/exe!";

  return utf8_to_path(path_data);
#elif __APPLE__
  char path_data[PATH_MAX];
  uint32_t size = sizeof(path_data);
  int count;

  if (_NSGetExecutablePath(path_data, &size) == 0)
    throw "Faileod to retrieve the executable's path from Mac OS!");

  return utf8_to_path(path_data);
#else
  std::cerr << "Warning: Don't know how to determine path to the current executable. Using './data'." << std::endl;
  return boost::filesystem::current_path() / "data";
#endif
#endif
}

path cResource_Manager :: Get_Game_Pixmaps_Dir()
{
  return Get_Data_Directory() / utf8_to_path(GAME_PIXMAPS_DIR);
}

path cResource_Manager :: Get_Game_Pixmap(std::string pixmap)
{
  return Get_Game_Pixmaps_Dir() / utf8_to_path(pixmap);
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

cResource_Manager *pResource_Manager = NULL;

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace SMC
