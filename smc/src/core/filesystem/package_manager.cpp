/***************************************************************************
 * package_manager.cpp  -  Package Manager
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

#include "package_manager.hpp"
#include "resource_manager.hpp"
#include "filesystem.hpp"
#include "../property_helper.hpp"
#include "../errors.hpp"

namespace fs = boost::filesystem;
namespace errc = boost::system::errc;

namespace SMC
{

/* *** *** *** *** *** *** cPackage_Manager *** *** *** *** *** *** *** *** *** *** *** */

cPackage_Manager :: cPackage_Manager( void )
{
	printf("Initializing Package Manager\n");
	Scan_Packages();
	Build_Search_Path();
}

cPackage_Manager :: ~cPackage_Manager( void )
{
}

void cPackage_Manager :: Scan_Packages( void )
{
	m_packages.clear();

	// Scan user data dir first so any user "packages.xml" will override the same in the game data dire
	Scan_Packages_Helper(pResource_Manager->Get_User_Data_Directory() / utf8_to_path("packages"), fs::path());
	Scan_Packages_Helper(pResource_Manager->Get_Game_Data_Directory() / utf8_to_path("packages"), fs::path());
}

std::vector<std::string> cPackage_Manager :: Get_Packages( void )
{
	std::vector<std::string> packages;
	std::map<std::string, PackageInfo>::const_iterator it;
	for(it = m_packages.begin(); it != m_packages.end(); it++)
	{
		packages.push_back( it->first );
	}
	std::sort(packages.begin(), packages.end());

	return packages;
}

void cPackage_Manager :: Set_Current_Package( const std::string& name )
{
	m_current_package = name;
	Build_Search_Path();
}

std::string cPackage_Manager :: Get_Current_Package( void )
{
	return m_current_package;
}

fs::path Get_Scripting_Path(const std::string& package, const std::string& script)
{
	// TODO: find script in user directory for package, if not then data directory for package.
	return fs::path();
}

std::vector<fs::path> cPackage_Manager :: Read_Path(fs::path path, bool parent /* = false */)
{
	int counter = 0;
	std::vector<fs::path> contents;
	for(std::vector<fs::path>::const_iterator it = m_search_path.begin(); it != m_search_path.end(); ++it)
	{
		fs::path subdir(*it / path);
		if(!(fs::exists(subdir) && fs::is_directory(subdir)))
			continue;

		fs::directory_iterator end_iter;
		for(fs::directory_iterator dir_iter(subdir); dir_iter != end_iter; ++dir_iter)
		{
			fs::path name(dir_iter->path().filename());
			if(std::find(contents.begin(), contents.end(), name) == contents.end())
				contents.push_back(name);
		}

		counter++;
		// If counter reaches 2, we are finished with the current package
		if(counter == 2 and !parent)
			break;
	}

	std::sort(contents.begin(), contents.end());
	return contents;
}

fs::path cPackage_Manager :: Get_Pixmap_Reading_Path(const std::string& pixmap)
{
	return Find_Reading_Path("pixmaps", utf8_to_path(pixmap));
}

fs::path cPackage_Manager :: Get_Level_Reading_Path(const std::string& level)
{
	fs::path result = Find_Reading_Path("levels", utf8_to_path(level + ".smclvl"));
	if(result == fs::path())
		result = Find_Reading_Path("levels", utf8_to_path(level + ".txt"));

	return result;
}

fs::path cPackage_Manager :: Get_Sound_Reading_Path(const std::string& sound)
{
	return Find_Reading_Path("sounds", utf8_to_path(sound));
}

fs::path cPackage_Manager :: Get_Campaign_Reading_Path(const std::string& campaign)
{
	return Find_Reading_Path("campaign", utf8_to_path(campaign));
}

fs::path cPackage_Manager :: Get_Overworld_Reading_Path(const std::string& overworld)
{
	return Find_Reading_Path("world", utf8_to_path(overworld));
}

fs::path cPackage_Manager :: Get_Music_Reading_Path(const std::string& music)
{
	return Find_Reading_Path("music", utf8_to_path(music));
}


fs::path cPackage_Manager :: Get_Level_Writing_Path(const std::string& level)
{
	return Find_Writing_Path("levels", utf8_to_path(level + ".smclvl"));
}

fs::path cPackage_Manager :: Get_Overworld_Writing_Path(const std::string& overworld)
{
	return Find_Writing_Path("world", utf8_to_path(overworld));
}

fs::path cPackage_Manager :: Get_Relative_Pixmap_Path(fs::path path)
{
	return Find_Relative_Path("pixmaps", path);
}

fs::path cPackage_Manager :: Get_Relative_Sound_Path(fs::path path)
{
	return Find_Relative_Path("sounds", path);
}

fs::path cPackage_Manager :: Get_Relative_Music_Path(fs::path path)
{
	return Find_Relative_Path("music", path);
}

void cPackage_Manager :: Scan_Packages_Helper( fs::path base, fs::path path )
{
	fs::path subdir(base / path);
	fs::directory_iterator end_iter;

	if(fs::exists(subdir) && fs::is_directory(subdir))
	{
		for(fs::directory_iterator dir_iter(subdir) ; dir_iter != end_iter ; ++dir_iter)
		{
			fs::path entry = dir_iter->path().filename();
			if(entry.extension() == fs::path(".smcpkg"))
			{
				entry.replace_extension("");
				std::string name = path_to_utf8(path / entry);

				if(m_packages.find(name) == m_packages.end())
				{
					m_packages[name] = Load_Package_Info(name);
					printf("Found package %s\n", name.c_str());
				}
			}
			else
			{
				Scan_Packages_Helper( base, path / entry );
			}
		}
	}
}

void cPackage_Manager :: Build_Search_Path ( void )
{
	m_search_path.clear();

	if(!m_current_package.empty())
	{
		std::vector<std::string> processed;
		Build_Search_Path_Helper( m_current_package, processed );	
	}

	// Add default data directories to search path
	m_search_path.push_back(pResource_Manager->Get_User_Data_Directory());
	m_search_path.push_back(pResource_Manager->Get_Game_Data_Directory());
}

void cPackage_Manager :: Build_Search_Path_Helper ( const std::string& package, std::vector<std::string>& processed )
{
	// Avoid search loops
	if(std::find(processed.begin(), processed.end(), package) != processed.end())
		return;

	std::map<std::string, PackageInfo>::const_iterator item(m_packages.find(package));
	if(item == m_packages.end())
		return;

	// Add user and game search paths
	processed.push_back(package);
	m_search_path.push_back(item->second.user_data_dir);
	m_search_path.push_back(item->second.game_data_dir);

	// Add any dependent package search paths
	for(std::vector<std::string>::const_iterator dep_it = item->second.dependencies.begin(); dep_it != item->second.dependencies.end(); ++dep_it)
		Build_Search_Path_Helper( *dep_it, processed );
}

PackageInfo cPackage_Manager :: Load_Package_Info( const std::string& package )
{
	fs::path path(utf8_to_path(package));
	path.replace_extension(".smcpkg");

	PackageInfo info;
	info.game_data_dir = pResource_Manager->Get_User_Data_Directory() / utf8_to_path("packages") / path;
	info.user_data_dir = pResource_Manager->Get_User_Data_Directory() / utf8_to_path("packages") / path;

	return info;
}

fs::path cPackage_Manager :: Find_Reading_Path(fs::path dir, fs::path resource)
{
	for(std::vector<fs::path>::const_iterator it = m_search_path.begin(); it != m_search_path.end(); ++it)
	{
		fs::path path = *it / dir / resource;
		if(fs::exists(path))
			return path;
	}

	return fs::path();
}

fs::path cPackage_Manager :: Find_Writing_Path(fs::path dir, fs::path resource)
{
	// Always return the user data path for the current package;
	fs::path base(pResource_Manager->Get_User_Data_Directory());
	if(!m_current_package.empty())
	{
		base = base / utf8_to_path("packages") / utf8_to_path(m_current_package);
		base.replace_extension(".smcpkg");
	}

	return base / dir / resource;
}

fs::path cPackage_Manager :: Find_Relative_Path(fs::path dir, fs::path path)
{
	for(std::vector<fs::path>::const_iterator it = m_search_path.begin(); it != m_search_path.end(); ++it)
	{
		fs::path subdir(*it / dir);
		if(subdir.filename() == ".")
			subdir.remove_filename();

		// Determine if abspath is under subdir
		// This was borrowed from http://stackoverflow.com/questions/15541263/how-to-determine-if-file-is-contained-by-path-with-boost-filesystem-v3
		fs::path tmp_path(path);
		tmp_path.remove_filename();

		if(std::distance(subdir.begin(), subdir.end()) > std::distance(tmp_path.begin(), tmp_path.end()))
			continue;

		if(!std::equal(dir.begin(), dir.end(), tmp_path.begin()))
			continue;

		// Found the path in the search path that it is under
		return fs::relative(subdir, path);
	}

	return fs::path();
}

//

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

cPackage_Manager *pPackage_Manager = NULL;

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace SMC
