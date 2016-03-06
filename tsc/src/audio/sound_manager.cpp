/***************************************************************************
 * sound_manager.cpp  -  Sound Handler/Manager
 *
 * Copyright © 2006 - 2011 Florian Richter
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

#include "../core/property_helper.hpp"
#include "../audio/sound_manager.hpp"

namespace fs = boost::filesystem;

namespace TSC {

/* *** *** *** *** *** *** *** *** Sound *** *** *** *** *** *** *** *** *** */

cSound::cSound(void) {}

cSound::~cSound(void)
{
    Free();
}

bool cSound::Load(const fs::path& filename)
{
    Free();

    if (m_buffer.loadFromFile(path_to_utf8(filename))) {
        m_filename = filename;
        return 1;
    }

    return 0;
}

void cSound::Free(void)
{
    m_filename.clear();
}


/* *** *** *** *** *** *** cSound_Manager *** *** *** *** *** *** *** *** *** *** *** */

cSound_Manager::cSound_Manager(void)
    : cObject_Manager<cSound>()
{
    m_load_count = 0;
}

cSound_Manager::~cSound_Manager(void)
{
    cSound_Manager::Delete_All();
}

cSound* cSound_Manager::Get_Pointer(const fs::path& path) const
{
    for (SoundList::const_iterator itr = objects.begin(); itr != objects.end(); ++itr) {
        cSound* obj = (*itr);

        if (obj->m_filename.compare(path) == 0) {
            // return first found
            return obj;
        }
    }

    // not found
    return NULL;
}

void cSound_Manager::Add(cSound* sound)
{
    m_load_count++;
    cObject_Manager<cSound>::Add(sound);
}

void cSound_Manager::Delete_Sounds(void)
{
    for (SoundList::iterator itr = objects.begin(); itr != objects.end(); ++itr) {
        cSound* obj = (*itr);

        delete obj;
        obj = NULL;
    }
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

cSound_Manager* pSound_Manager = NULL;

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace TSC
