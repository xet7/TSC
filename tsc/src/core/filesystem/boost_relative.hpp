/***************************************************************************
 * boost_relative.h
 *
 * Taken from http://stackoverflow.com/questions/10167382/boostfilesystem-get-relative-path
 ***************************************************************************/

/*
 * This file adds a function make_relative() to boost::filesystem that
 * allows us to create a relative path from a given “root”, i.e. the
 * common parts of both paths are missing in the returned path object.
 */

#ifndef SMC_BOOST_RELATIVE_HPP
#define SMC_BOOST_RELATIVE_HPP
#include <boost/filesystem.hpp>

namespace boost {

    namespace filesystem {

        template < >
        path& path::append< typename path::iterator >(typename path::iterator begin, typename path::iterator end, const codecvt_type& cvt);

        // Return path when appended to a_From will resolve to same as a_To
        boost::filesystem::path relative(boost::filesystem::path a_From, boost::filesystem::path a_To);
    }
}
#endif
