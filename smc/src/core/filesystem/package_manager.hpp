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
	void Scan_Packages(void);
	// Get the list of known packages
	std::vector<std::string> Get_Packages( void );

	// Set the current package
	void Set_Current_Package( const std::string& name );
	std::string Get_Current_Package( void );

	// Create user paths
	void Init_User_Paths( void );
	
	// Return the path based on the seach path, or fs::path() if no more search paths
	boost::filesystem::path Get_User_Data_Path(int pos = 0);
	boost::filesystem::path Get_Game_Data_Path(int pos = 0);

	boost::filesystem::path Get_User_Level_Path(int pos = 0);
	boost::filesystem::path Get_Game_Level_Path(int pos = 0);
 
	boost::filesystem::path Get_User_Campaign_Path(int pos = 0);
	boost::filesystem::path Get_Game_Campaign_Path(int pos = 0);

	boost::filesystem::path Get_User_World_Path(int pos = 0);
	boost::filesystem::path Get_Game_World_Path(int pos = 0);

	// Get a script
	boost::filesystem::path Get_Scripting_Path(const std::string& package, const std::string& script);

	// Find resources for reading
	boost::filesystem::path Get_Pixmap_Reading_Path(const std::string& pixmap);
	boost::filesystem::path Get_Level_Reading_Path(const std::string& level);
	boost::filesystem::path Get_Sound_Reading_Path(const std::string& sound);
	boost::filesystem::path Get_Campaign_Reading_Path(const std::string& campaign);
	boost::filesystem::path Get_Overworld_Reading_Path(const std::string& overworld);
	boost::filesystem::path Get_Music_Reading_Path(const std::string& music);
	
	// Find resources for writing
	boost::filesystem::path Get_Level_Writing_Path(const std::string& level);
	boost::filesystem::path Get_Overworld_Writing_Path(const std::string& overworld);

	// Find the relative path (for saving relative paths to files)
	boost::filesystem::path Get_Relative_Pixmap_Path(boost::filesystem::path path);
	boost::filesystem::path Get_Relative_Sound_Path(boost::filesystem::path path);
	boost::filesystem::path Get_Relative_Music_Path(boost::filesystem::path path);


private:
	void Scan_Packages_Helper( boost::filesystem::path base, boost::filesystem::path path );
	void Build_Search_Path( void );
	void Build_Search_Path_Helper( const std::string& package, std::vector<std::string>& processed );
	PackageInfo Load_Package_Info( const std::string& package );

	boost::filesystem::path Find_Reading_Path(boost::filesystem::path dir, boost::filesystem::path resource);
	boost::filesystem::path Find_Writing_Path(boost::filesystem::path dir, boost::filesystem::path resource);
	boost::filesystem::path Find_Relative_Path(boost::filesystem::path dir, boost::filesystem::path path);

	std::map <std::string, PackageInfo> m_packages;
	std::string m_current_package;
	std::vector<boost::filesystem::path> m_search_path;
};

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

// Package Manager
extern cPackage_Manager *pPackage_Manager;

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace SMC

#endif

