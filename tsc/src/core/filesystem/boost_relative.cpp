/***************************************************************************
 * boost_relative.cpp - Implementation of boost::filesystem::relvative()
 *
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

#include "boost_relative.hpp"

namespace boost {
namespace filesystem {

template < >
path& path::append< typename path::iterator >(typename path::iterator begin, typename path::iterator end, const codecvt_type& cvt)
{
    for (; begin != end ; ++begin)
        *this /= *begin;
    return *this;
}

boost::filesystem::path relative(boost::filesystem::path a_From, boost::filesystem::path a_To)
{
    a_From = boost::filesystem::absolute(a_From);
    a_To = boost::filesystem::absolute(a_To);
    boost::filesystem::path ret;
    boost::filesystem::path::const_iterator itrFrom(a_From.begin()), itrTo(a_To.begin());
    // Find common base
    for (boost::filesystem::path::const_iterator toEnd(a_To.end()), fromEnd(a_From.end()) ; itrFrom != fromEnd && itrTo != toEnd && *itrFrom == *itrTo; ++itrFrom, ++itrTo);
    // Navigate backwards in directory to reach previously found base
    for (boost::filesystem::path::const_iterator fromEnd(a_From.end()); itrFrom != fromEnd; ++itrFrom) {
        if ((*itrFrom) != ".")
            ret /= "..";
    }
    // Now navigate down the directory branch
    ret.append(itrTo, a_To.end());
    return ret;

}

}
}
