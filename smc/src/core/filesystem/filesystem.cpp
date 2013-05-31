/***************************************************************************
 * filesystem.cpp  -  File System
 *
 * Copyright (C) 2005 - 2011 Florian Richter
 ***************************************************************************/
/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../../core/filesystem/filesystem.h"
#include "../../core/game_core.h"
// boost filesystem
#include "boost/filesystem/convenience.hpp"
namespace fs = boost::filesystem;
// needed for the stat function and to get the user directory on unix
#include <sys/stat.h>
#include <sys/types.h>
#if _WIN32
// needed to get the user directory (SHGetFolderPath)
#include <shlobj.h>
#endif

namespace SMC
{

/* *** *** *** *** *** *** cResource_Manager *** *** *** *** *** *** *** *** *** *** *** */

fs::path Trim_Filename( fs::path filename, bool keep_dir /* = 1 */, bool keep_end /* = 1 */ )
{
	if (!keep_dir)
		filename = filename.filename();
	if (!keep_end)
		filename.replace_extension("");

	return filename;
}

bool File_Exists( const fs::path &filename )
{
  fs::file_type type = fs::status(filename).type();

	return type == fs::regular_file || type == fs::symlink_file;
}

bool Dir_Exists( const fs::path &dir )
{
  fs::file_type type = fs::status(dir).type();

	return type == fs::directory_file || type == fs::symlink_file;
}

bool Delete_Dir( const std::string &dir )
{
// fixme : boost should use a codecvt_facet but for now we convert to UCS-2
#ifdef _WIN32
	return RemoveDirectoryW( utf8_to_ucs2( dir ).c_str() ) != 0;
#else
	return rmdir( dir.c_str() ) == 0;
#endif
}

size_t Get_File_Size( const std::string &filename )
{
	struct stat file_info; 

	// if file exists
	if( stat( filename.c_str(), &file_info ) == 0 )
	{
		// if regular file
	#ifdef _WIN32
		if( (file_info.st_mode & S_IFREG) > 0 )
	#else
		if( !S_ISDIR(file_info.st_mode) )
	#endif
		{
			// return size
			return file_info.st_size;
		}
	}

	return 0;
}

void Convert_Path_Separators( std::string &str )
{
	for( std::string::iterator itr = str.begin(); itr != str.end(); ++itr )
	{
		// convert it
		if( *itr == '\\' || *itr == '!' )
		{
			*itr = '/';
		}
	}
}

vector<fs::path> Get_Directory_Files( const fs::path &dir, const std::string &file_type /* = "" */, bool with_directories /* = false */, bool search_in_sub_directories /* = true */ )
{
	vector<fs::path> valid_files;
	fs::path extension = utf8_to_path(file_type);

	fs::directory_iterator end_iter; // No-args constructor makes an end-iter according to docs

	// load all available objects
	for( fs::directory_iterator dir_itr( dir ); dir_itr != end_iter; ++dir_itr )
	{
		try
		{
      const fs::path this_path = dir_itr->path();

			// if directory
			if( fs::is_directory( *dir_itr ) )
			{
				// ignore hidden directories and . and ..
				if (path_to_utf8(this_path.filename()).find(".") == 0)
				{
					continue;
				}

				if( with_directories )
				{
					valid_files.push_back(this_path);
				}

				// load all items from the sub-directory
				if( search_in_sub_directories )
				{
					vector<fs::path> new_valid_files = Get_Directory_Files( this_path, file_type, with_directories );
					valid_files.insert( valid_files.end(), new_valid_files.begin(), new_valid_files.end() );
				}
			}
			// valid file
			else if( extension.empty() || this_path.extension() == extension )
			{
				valid_files.push_back( this_path );
			}
		}
		catch( const std::exception &ex )
		{
			printf( "%s %s\n", dir_itr->path().string().c_str(), ex.what() );
		}
	}

	return valid_files;
}

boost::filesystem::path Get_Temp_Directory( void )
{
	return boost::filesystem::temp_directory_path();
}

// FIXME: This should return a boost::filesystem::path!
std::string Get_User_Directory( void )
{
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

	return str_path + "/smc/";
#elif __unix__
	return (std::string)getenv( "HOME" ) + "/.smc/";
#elif __APPLE__
	return (std::string)getenv( "HOME" ) + "/Library/Application Support/smc/";
#else
	return "";
#endif
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace SMC
