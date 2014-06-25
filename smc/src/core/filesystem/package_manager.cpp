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
	Rescan_Packages();
}

cPackage_Manager :: ~cPackage_Manager( void )
{
}

void cPackage_Manager :: Rescan_Packages( void )
{
	m_packages.clear();

	Scan_Packages_Helper(pResource_Manager->Get_Game_Data_Directory() / utf8_to_path("packages"), fs::path(), m_packages);
	Scan_Packages_Helper(pResource_Manager->Get_User_Data_Directory() / utf8_to_path("packages"), fs::path(), m_packages);
	std::sort(m_packages.begin(), m_packages.end());
}

std::vector<std::string> cPackage_Manager :: Get_Packages( void )
{
	return m_packages;
}

void cPackage_Manager :: Scan_Packages_Helper( fs::path base, fs::path path, std::vector<std::string>& packages)
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

				if(std::find(packages.begin(), packages.end(), name) == packages.end())
				{
					packages.push_back(name);
					printf("Found package %s\n", name.c_str());
				}
			}
			else
			{
				Scan_Packages_Helper( base, path / entry, packages );
			}
		}
	}
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

cPackage_Manager *pPackage_Manager = NULL;

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace SMC
