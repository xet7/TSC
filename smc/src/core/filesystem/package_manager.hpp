/***************************************************************************
 * package_manager.hpp
 *
 * Copyright © 2009 - 2014 The SMC Contributors
 ***************************************************************************/
/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SMC_PACKAGE_MANAGER_HPP
#define SMC_PACKAGE_MANAGER_HPP

#include "../../core/global_basic.hpp"
#include "../../core/global_game.hpp"

namespace SMC
{

	struct PackageInfo {
		std::string name;
		std::vector<std::string> dependencies;
		boost::filesystem::path game_data_dir;
		boost::filesystem::path user_data_dir;
		boost::filesystem::path user_cache_dir;
	};


/* *** *** *** *** *** cPackage_Manager *** *** *** *** *** *** *** *** *** *** *** *** */

class cPackage_Manager
{
public:
	cPackage_Manager( void );
	~cPackage_Manager( void );

	// Scan the available list of packages
	void Rescan_Packages(void);
	// Get the list of known packages
	std::vector<std::string> Get_Packages(void);


private:
	void Scan_Packages_Helper(boost::filesystem::path base, boost::filesystem::path path, std::vector<std::string>& packages);

	std::vector<std::string> m_packages;

};

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

// Package Manager
extern cPackage_Manager *pPackage_Manager;

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace SMC

#endif

