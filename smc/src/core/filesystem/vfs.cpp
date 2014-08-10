/***************************************************************************
 * vfs.cpp  -  Virtual File System
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

#include <physfs.h>

#include "vfs.hpp"
#include "resource_manager.hpp"
#include "filesystem.hpp"
#include "../../user/preferences.hpp"
#include "../property_helper.hpp"
#include "../errors.hpp"

namespace fs = boost::filesystem;
namespace errc = boost::system::errc;

namespace SMC
{

/* *** *** *** *** *** cVfs_streambuf *** *** *** *** *** *** *** *** *** *** */
class cVfs_streambuf : public std::streambuf
{
public:
	explicit cVfs_streambuf(PHYSFS_File* f, size_t size=2048, size_t putback=1) :
		m_file(f),
		m_putback(std::max(putback, size_t(1))),
		m_buffer(std::max(size, putback) + putback)
	{
		char* end = &m_buffer.front() + m_buffer.size();
		setg(end, end, end);
	}

	~cVfs_streambuf()
	{
		PHYSFS_close(m_file);
	}

private:
	cVfs_streambuf(const cVfs_streambuf&);
	cVfs_streambuf& operator=(const cVfs_streambuf&);

	PHYSFS_File* m_file;
	size_t m_putback;
	std::vector<char> m_buffer;

protected:
	std::streambuf::int_type underflow()
	{
		if(gptr() < egptr())
			return traits_type::to_int_type(*gptr());

		char* base = &m_buffer.front();
		char* start = base;

		if(eback() == base)
		{
			size_t diff = std::min(m_putback, static_cast<size_t>(egptr() - base));
			std::memmove(base, egptr() - diff, diff);
			start += diff;
		}

		// Read data from handle
		PHYSFS_sint64 n = PHYSFS_read(m_file, start, 1, m_buffer.size() - (start - base));
		if(n <= 0) // todo: -1 is an error
			return traits_type::eof();

		setg(base, start, start + static_cast<size_t>(n));
		return traits_type::to_int_type(*gptr());
	}
};

/* *** *** *** *** *** *** cVfs *** *** *** *** *** *** *** *** *** *** *** */

cVfs :: cVfs( const char *argv0 ) : m_mount_index(0)
{
	std::cout << "Initializing VFS" << std::endl;
	PHYSFS_init(argv0);
}

cVfs :: ~cVfs( void )
{
	PHYSFS_deinit();
}

std::string cVfs :: Mount(fs::path dir)
{
	// First check if this path has already been mounted, and if so, return the mount point
	if(m_mount_points.find(dir) != m_mount_points.end())
		return m_mount_points[dir];

	// Otherwise, create the mount point in PhysFS
	std::string mount_point = "/mounts/" + int_to_string(m_mount_index);
	m_mount_index++;

	PHYSFS_mount(path_to_utf8(dir).c_str(), mount_point.c_str(), 1);
	m_mount_points[dir] = mount_point;

	return mount_point;
}

bool cVfs :: Exists(fs::path path)
{
	std::string physfs_path = Find(path);
	if(!physfs_path.empty())
	{
		return PHYSFS_exists(physfs_path.c_str()) != 0;
	}

	return false;
}

bool cVfs :: File_Exists(fs::path path)
{
	std::string physfs_path = Find(path);
	if(!physfs_path.empty())
	{
		return PHYSFS_exists(physfs_path.c_str()) != 0 && PHYSFS_isDirectory(physfs_path.c_str()) == 0;
	}

	return false;
}

bool cVfs :: Directory_Exists(fs::path path)
{
	std::string physfs_path = Find(path);
	if(!physfs_path.empty())
	{
		return PHYSFS_exists(physfs_path.c_str()) != 0 && PHYSFS_isDirectory(physfs_path.c_str()) != 0;
	}

	return false;
}



std::istream* cVfs :: Open_Stream(fs::path file)
{
	std::string physfs_path = Find(file);

	if(!physfs_path.empty())
	{
		PHYSFS_File* f = PHYSFS_openRead(physfs_path.c_str());
		if(f)
		{
			return new std::istream(new cVfs_streambuf(f));
		}
	}

	return NULL;
}

std::string cVfs :: Find(fs::path path)
{
	fs::path internal;
	std::string result;
	
	path = fs::absolute(path);
	while(!(path.empty() || fs::exists(path)))
	{
		internal = path.filename() / internal;
		path = path.parent_path();
	}

	if(path.empty())
		return result;

	if(internal.empty())
	{
		// internal is not set, so it may not be a zip file
		// or it could be the root of zip file
		
		if(m_mount_points.find(path) != m_mount_points.end())
		{
			// mount point already exists
			result = m_mount_points[path];
		}
		else
		{
			// mount point doesn't exist
			result = Mount(path.root_path()) + "/" + path_to_physfs(path.relative_path());
		}
	}
	else
	{
		// internal is set, so path must be a zip file
		if(fs::is_regular_file(path))
		{
			result = Mount(path) + "/" + path_to_physfs(internal);
		}
	}

	return result;
}

/* static */
std::string cVfs :: path_to_physfs(fs::path path)
{
	// TODO: ensure result are in UTF8 for physfs and needs to use '/' for separators
	return path.generic_string();
}

/* static */
fs::path cVfs :: physfs_to_path(std::string physfs_path)
{
	return utf8_to_path(physfs_path);
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

cVfs *pVfs = NULL;

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace SMC

