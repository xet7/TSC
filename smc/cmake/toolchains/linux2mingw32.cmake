# CMake toolchain definition file for cross-compiling
# SMC from a Linux host to the win32 target, using the
# Minimalist GNU for Windows (MinGW) system.
set(CMAKE_SYSTEM_NAME Windows)

########################################
# CUSTOMIZE SECTION
########################################

# Replace these with the actual names of your crosscompilers.
set(CMAKE_C_COMPILER i686-pc-mingw32-gcc)
set(CMAKE_CXX_COMPILER i686-pc-mingw32-g++)
set(CMAKE_RC_COMPILER i686-pc-mingw32-windres)

# Replace this with the root path(s) of your cross-compilation
# environment. I recommend MXE (http://mxe.cc) for setting up one.
# $ENV{HOME} gets replaced with the absolute path to your home directory.
# Examples:
#   set(CMAKE_FIND_ROOT_PATH "/opt/crossenv")
#   set(CMAKE_FIND_ROOT_PATH "$ENV{HOME}/crossenv" "$ENV{HOME}/otherstuffhere")
#   set(CMAKE_FIND_ROOT_PATH "/opt/crossenvs/i686-mingw32")
set(CMAKE_FIND_ROOT_PATH "$ENV{HOME}/win32/mxe/usr/i686-pc-mingw32")

########################################
# END OF CUSTOMIZE SECTION
########################################

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
