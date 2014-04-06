/***************************************************************************
 * resource_manager.cpp  -  Resource Manager
 *
 * Copyright © 2009 - 2011 The SMC Contributors
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

#include "resource_manager.hpp"
#include "filesystem.hpp"
#include "../property_helper.hpp"

namespace fs = boost::filesystem;

namespace SMC
{

/* *** *** *** *** *** *** cResource_Manager *** *** *** *** *** *** *** *** *** *** *** */

cResource_Manager :: cResource_Manager( void )
{
	m_forced_user_data_dir = fs::path();
}

cResource_Manager :: ~cResource_Manager( void )
{

}

void cResource_Manager :: Init_User_Directory( void )
{
	fs::path user_data_dir = Get_User_Data_Directory();

	// Create user directory
	if( !Dir_Exists( user_data_dir ) )
	{
		// first run if not available
		debug_print("Creating user directory at '%s'\n", path_to_utf8(user_data_dir).c_str());
		fs::create_directory( user_data_dir );
	}
	// Create savegame directory
	if( !Dir_Exists( Get_User_Savegame_Directory() ))
	{
		fs::create_directory( Get_User_Savegame_Directory() );
	}
	// Create screenshot directory
	if( !Dir_Exists( Get_User_Screenshot_Directory() ) )
	{
		fs::create_directory( Get_User_Screenshot_Directory() );
	}
	// Create level directory
	if( !Dir_Exists( Get_User_Level_Directory() ) )
	{
		fs::create_directory( Get_User_Level_Directory() );
	}
	// Create world directory
	if( !Dir_Exists( Get_User_World_Directory() ) )
	{
		fs::create_directory( Get_User_World_Directory() );
	}
	// Create campaign directory
	if( !Dir_Exists( Get_User_Campaign_Directory() ) )
	{
		fs::create_directory( Get_User_Campaign_Directory() );
	}
	// Create cache directory
	if( !Dir_Exists( Get_User_Imgcache_Directory() ) )
	{
		fs::create_directory( Get_User_Imgcache_Directory() );
	}
}

void cResource_Manager :: Force_User_Directory( const fs::path &dir )
{
	m_forced_user_data_dir = dir;
}

fs::path cResource_Manager :: Get_Data_Directory( void )
{
#ifdef FIXED_DATA_DIR
  return utf8_to_path(FIXED_DATA_DIR);
#else
#ifdef _WIN32
  wchar_t path_data[MAX_PATH + 1];

  if (GetModuleFileNameW(NULL, path_data, MAX_PATH) == 0)
    throw "Failed to retrieve the executable's path from the Win32API!";

  std::string utf8_path = ucs2_to_utf8(path_data);
  //Convert_Path_Separators(utf8_path);

  return utf8_to_path(utf8_path).parent_path().parent_path() / "share" / "smc";
#elif __linux
	char path_data[PATH_MAX];
	int count;

	count = readlink("/proc/self/exe", path_data, PATH_MAX);
	if (count < 0)
		throw "Failed to retrieve the executable's path from /proc/self/exe!";

	return utf8_to_path(std::string(path_data, count)).parent_path().parent_path() / "share" / "smc";
#elif __APPLE__
  char path_data[PATH_MAX];
  uint32_t size = sizeof(path_data);
  int count;

  if (_NSGetExecutablePath(path_data, &size) == 0)
    throw "Faileod to retrieve the executable's path from Mac OS!");

  return utf8_to_path(path_data).parent_path().parent_path() / "share" / "smc";
#else
  std::cerr << "Warning: Don't know how to determine path to the current executable. Using './data'." << std::endl;
  return boost::filesystem::current_path() / "data";
#endif
#endif
}

fs::path cResource_Manager :: Get_Game_Pixmaps_Directory()
{
  return Get_Data_Directory() / utf8_to_path(GAME_PIXMAPS_DIR);
}

fs::path cResource_Manager :: Get_Game_Pixmap(std::string pixmap)
{
  return Get_Game_Pixmaps_Directory() / utf8_to_path(pixmap);
}

fs::path cResource_Manager :: Get_User_Data_Directory()
{
	// If the directory has been forced, return that one and nothing else.
	if (!m_forced_user_data_dir.empty())
		return m_forced_user_data_dir;

	// Otherwise, retrieve the default directory from the system.
#ifdef _WIN32
	wchar_t path_appdata[MAX_PATH + 1];

	if( FAILED( SHGetFolderPathW( NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, path_appdata ) ) )
	{
		printf( "Error : Couldn't get Windows user data directory. Defaulting to the Application directory.\n" );
		return "";
	}

	std::string str_path = ucs2_to_utf8( path_appdata );
	Convert_Path_Separators( str_path );

	/*std::wstring str = utf8_to_ucs2( str_path );
	str.insert( str.length(), L"\n" );
	HANDLE std_out = GetStdHandle( STD_OUTPUT_HANDLE );
	unsigned long chars;
	WriteConsole( std_out, str.c_str(), lstrlen(str.c_str()), &chars, NULL );*/

	return utf8_to_path(str_path + "/smc/");
#elif __unix__
	return utf8_to_path((std::string)getenv( "HOME" ) + "/.smc/");
#elif __APPLE__
	return utf8_to_path((std::string)getenv( "HOME" ) + "/Library/Application Support/smc/");
#else
#error Dont know how to determine the user data directory on this patform!
#endif
}

fs::path cResource_Manager :: Get_User_Level_Directory()
{
  return Get_User_Data_Directory() / utf8_to_path(USER_LEVEL_DIR);
}

fs::path cResource_Manager :: Get_User_Level(std::string filename)
{
  return Get_User_Level_Directory() / utf8_to_path(filename);
}

fs::path cResource_Manager :: Get_User_Savegame_Directory()
{
  return Get_User_Data_Directory() / utf8_to_path(USER_SAVEGAME_DIR);
}

fs::path cResource_Manager :: Get_User_Screenshot_Directory()
{
  return Get_User_Data_Directory() / utf8_to_path(USER_SCREENSHOT_DIR);
}

fs::path cResource_Manager :: Get_User_World_Directory()
{
  return Get_User_Data_Directory() / utf8_to_path(USER_WORLD_DIR);
}

fs::path cResource_Manager :: Get_User_Campaign_Directory()
{
  return Get_User_Data_Directory() / utf8_to_path(USER_CAMPAIGN_DIR);
}

fs::path cResource_Manager :: Get_User_Imgcache_Directory()
{
  return Get_User_Data_Directory() / utf8_to_path(USER_IMGCACHE_DIR);
}

fs::path cResource_Manager :: Get_Game_Schema_Directory()
{
  return Get_Data_Directory() / utf8_to_path(GAME_SCHEMA_DIR);
}

fs::path cResource_Manager :: Get_Game_Schema(std::string schema)
{
  return Get_Game_Schema_Directory() / utf8_to_path(schema);
}

fs::path cResource_Manager :: Get_Game_Level_Directory()
{
  return Get_Data_Directory() / utf8_to_path(GAME_LEVEL_DIR);
}

fs::path cResource_Manager :: Get_Game_Level(std::string level)
{
  return Get_Game_Level_Directory() / utf8_to_path(level);
}

fs::path cResource_Manager :: Get_Game_Translation_Directory()
{
	return Get_Data_Directory() / utf8_to_path(GAME_TRANSLATION_DIR);
}

fs::path cResource_Manager :: Get_Game_Translation(std::string transname)
{
	return Get_Game_Translation_Directory() / utf8_to_path(transname);
}

fs::path cResource_Manager :: Get_Game_Sounds_Directory()
{
	return Get_Data_Directory() / utf8_to_path(GAME_SOUNDS_DIR);
}

fs::path cResource_Manager :: Get_Game_Sound(std::string sound)
{
	return Get_Game_Sounds_Directory() / utf8_to_path(sound);
}

fs::path cResource_Manager :: Get_Game_Campaign_Directory()
{
	return Get_Data_Directory() / utf8_to_path(GAME_CAMPAIGN_DIR);
}

fs::path cResource_Manager :: Get_Game_Campaign(std::string campaign)
{
	return Get_Game_Campaign_Directory() / utf8_to_path(campaign);
}

fs::path cResource_Manager :: Get_Game_Overworld_Directory()
{
	return Get_Data_Directory() / utf8_to_path(GAME_OVERWORLD_DIR);
}

fs::path cResource_Manager :: Get_Game_Overworld(std::string overworld)
{
	return Get_Game_Overworld_Directory() / utf8_to_path(overworld);
}

fs::path cResource_Manager :: Get_Gui_Scheme_Directory()
{
	return Get_Data_Directory() / utf8_to_path(GUI_SCHEME_DIR);
}

fs::path cResource_Manager :: Get_Gui_Imageset_Directory()
{
	return Get_Data_Directory() / utf8_to_path(GUI_IMAGESET_DIR);
}

fs::path cResource_Manager :: Get_Gui_Font_Directory()
{
	return Get_Data_Directory() / utf8_to_path(GUI_FONT_DIR);
}

fs::path cResource_Manager :: Get_Gui_LookNFeel_Directory()
{
	return Get_Data_Directory() / utf8_to_path(GUI_LOOKNFEEL_DIR);
}

fs::path cResource_Manager :: Get_Gui_Layout_Directory()
{
	return Get_Data_Directory() / utf8_to_path(GUI_LAYOUT_DIR);
}

fs::path cResource_Manager :: Get_Game_Music_Directory()
{
	return Get_Data_Directory() / utf8_to_path(GAME_MUSIC_DIR);
}

fs::path cResource_Manager :: Get_Game_Music(std::string music)
{
	return Get_Game_Music_Directory() / utf8_to_path(music);
}

fs::path cResource_Manager :: Get_Game_Editor_Directory()
{
	return Get_Data_Directory() / utf8_to_path(GAME_EDITOR_DIR);
}

fs::path cResource_Manager :: Get_Game_Editor(std::string editor)
{
	return Get_Game_Editor_Directory() / utf8_to_path(editor);
}

fs::path cResource_Manager :: Get_Game_Scripting_Directory()
{
	return Get_Data_Directory() / utf8_to_path(GAME_SCRIPTING_DIR);
}

fs::path cResource_Manager :: Get_Game_Scripting(std::string script)
{
	return Get_Game_Scripting_Directory() / utf8_to_path(script);
}

fs::path cResource_Manager :: Get_Game_Icon_Directory()
{
	return Get_Data_Directory() / utf8_to_path(GAME_ICON_DIR);
}

fs::path cResource_Manager :: Get_Game_Icon(std::string icon)
{
	return Get_Game_Icon_Directory() / utf8_to_path(icon);
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

cResource_Manager *pResource_Manager = NULL;

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace SMC
