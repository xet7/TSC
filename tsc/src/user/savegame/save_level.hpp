/***************************************************************************
 * save_level.cpp - Handler for saving a cLevel instance to a savegame.
 *
 * Copyright © 2003 - 2011 Florian Richter
 * Copyright © 2013 - 2014 The TSC Contributors
 ***************************************************************************/
/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TSC_SAVEGAME_SAVE_LEVEL_HPP
#define TSC_SAVEGAME_SAVE_LEVEL_HPP
#include "../objects/sprite.hpp"

namespace TSC {
    /* *** *** *** *** *** *** *** cSave_Level *** *** *** *** *** *** *** *** *** *** */
    /**
     * Represents a cLevel instance in the savegame containing a list of
     * all spawned sprites. Note a single savegame may include multiple
     * levels when a user saves while or after he visited a sublevel
     */
    class cSave_Level {
    public:
        cSave_Level(void);
        ~cSave_Level(void);

        void Save_To_Node(xmlpp::Element* p_parent_node);

        std::string m_name;
        /// True if this is the active level.
        bool is_active;
        // player position is only set if level is the active one
        float m_level_pos_x;
        float m_level_pos_y;

        /// List of objects that originate from the level XML.
        cSprite_List m_regular_objects;
        /// List of spawned objects (i.e. not from the level XML).
        cSprite_List m_spawned_objects;

        // Data a script writer wants to store
        std::string m_mruby_data;
    };

    typedef vector<cSave_Level*> Save_LevelList;
}

#endif
