cmake_minimum_required(VERSION 2.8)
enable_language(CXX)
project(SMC)

# Ensure our custom CMake modules get found
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Some CMake modules we use
include(ExternalProject)

###############################################
# Flags & Options

option(ENABLE_MRUBY "Enable the MRuby scripting engine" ON)
#FIXME: Actually use ENABLE_MRUBY

# Extra flags for debugging SMC, additional to those
# already added by CMake itself when doing a debug
# build (e.g. -g is added for GCC by CMake).
if(${CMAKE_BUILD_TYPE} MATCHES Debug)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG=1")
endif()

# On Windows we don’t know the installation directly at compile
# time, therefore we cannot define DATA_DIR yet. Not defining
# this macro causes global_game.h to define it to "data", i.e.
# SMC will look for a path "data/" in the current working directory.
# FIXME: This should probably rather check if a standalone build
# is requested rather as it is not really windows-specific.
if (NOT WIN32)
  add_definitions(
    "-DDATA_DIR=\"${CMAKE_INSTALL_PREFIX}/share/smc\""
    )
endif()


# Configuration options for the library finding
# functions.
set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_STATIC_RUNTIME OFF)

########################################
# Cross-compilation

if (TOOLCHAIN_PREFIX)
  message(STATUS "Cross-compilation detected: ${TOOLCHAIN_PREFIX}")
endif()

###############################################
# Check for the actual libraries and programs.

if (ENABLE_MRUBY)
  # Rake, for building mruby
  find_program(RAKE_EXECUTABLE rake)
  find_program(GPERF_EXECUTABLE gperf)
  if(RAKE_EXECUTABLE)
    message(STATUS "Found rake: ${RAKE_EXECUTABLE}")
  else()
    message(ERROR "Cannot find a rake executable")
  endif()
  if(GPERF_EXECUTABLE)
    message(STATUS "Found gperf: ${GPERF_EXECUTABLE}")
  else()
    message(ERROR "Cannot find a gperf executable")
  endif()
endif()

# Several libraries we depend on
# FIXME: Detect CEGUI via pkg-config when the NullRenderer dependency is gone
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(CEGUI COMPONENTS NULL OPENGL REQUIRED)
find_package(Gettext)
find_package(BISON REQUIRED)

if (WIN32)
  find_package(LibIntl REQUIRED)
  find_package(Boost 1.50.0
    COMPONENTS filesystem chrono thread_win32 system
    REQUIRED)

  # Boost-thread and CEGUI need to be explicitely told that they’re
  # linked in statically on Windows.
  add_definitions(-DBOOST_THREAD_USE_LIB)
else()
  find_package(Boost 1.50.0
    COMPONENTS filesystem chrono thread system
    REQUIRED)
endif()

# pkg-config is really the better way to check for libs.
# Unfortunetly, the above libraries don’t support it
# properly.
pkg_check_modules(PNG REQUIRED libpng<=1.5.14) # SDL doesn’t like libpng 1.6 yet as it seems
pkg_check_modules(SDL REQUIRED sdl)
pkg_check_modules(SDL_IMAGE REQUIRED SDL_image)
pkg_check_modules(SDL_MIXER REQUIRED SDL_mixer)
pkg_check_modules(SDL_TTF REQUIRED SDL_ttf)
pkg_check_modules(FREEIMAGE REQUIRED freeimage)
pkg_check_modules(PCRE REQUIRED libpcre)

###############################################
# Definitions etc.

# Add any definitiony required by libraries
add_definitions(
  ${CEGUI_DEFINITIONS}
  ${PNG_DEFINITIONS}
  )

# Add all our libraries to our -I-nclude path
include_directories(
  ${OPENGL_INCLUDE_DIR}
  ${CEGUI_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${PNG_INCLUDE_DIRS}
  ${SDL_INCLUDE_DIRS}
  ${SDL_IMAGE_INCLUDE_DIRS}
  ${SDL_MIXER_INCLUDE_DIRS}
  ${SDL_TTF_INCLUDE_DIRS}
  ${FREEIMAGE_INCLUDE_DIRS}
  ${PCRE_INCLUDE_DIRS}
  )

###############################################
# mruby / Lua

if (ENABLE_MRUBY)
  # Tell CMake how to compile mruby
  set(MRUBY_ROOT_DIR ${SMC_SOURCE_DIR}/../mruby/mruby)

  ExternalProject_Add(
    mruby
    PREFIX "${SMC_BINARY_DIR}/mruby"
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""
    SOURCE_DIR "${MRUBY_ROOT_DIR}"
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${RAKE_EXECUTABLE} MRUBY_CONFIG=${SMC_SOURCE_DIR}/mruby_smc_build_config.rb CROSSCOMPILE_TARGET=${TOOLCHAIN_PREFIX}
    INSTALL_COMMAND ""
  )
  include_directories(${MRUBY_ROOT_DIR}/include)

  if(TOOLCHAIN_PREFIX)
    set(MRuby_LIBRARIES "${MRUBY_ROOT_DIR}/build/${TOOLCHAIN_PREFIX}/lib/libmruby.a" "${MRUBY_ROOT_DIR}/build/${TOOLCHAIN_PREFIX}/lib/libmruby_core.a")
  else()
    set(MRuby_LIBRARIES "${MRUBY_ROOT_DIR}/build/host/lib/libmruby.a" "${MRUBY_ROOT_DIR}/build/host/lib/libmruby_core.a")
  endif()
else() # Disable it
  set(MRuby_LIBRARIES "")
endif()

# Recurse for Lua and place the resulting
# liblua.a next to us.
add_subdirectory(lua ${SMC_BINARY_DIR}/lua)
include_directories(${SMC_SOURCE_DIR}/lua/lua-5.2.1/src)

###############################################
# Configuration header

configure_file(${SMC_SOURCE_DIR}/config.h.in
  ${SMC_BINARY_DIR}/config.h)
include_directories(${SMC_BINARY_DIR})

###############################################
# Source files

# For now, just assume ALL cpp files to be requisites
# of SMC.
file(GLOB_RECURSE smc_sources
  "src/*.cpp"
  "src/*.hpp"
  "src/*.h"
  )

###############################################
# Targets

# Now add our build targets and their dependencies.
add_executable(smc ${smc_sources})
if (ENABLE_MRUBY)
  add_dependencies(smc mruby)
endif()
# TODO: Make the mruby config automatically include the correct path to the PCRE library
if (WIN32)
  target_link_libraries(smc
    ${CEGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${FREEIMAGE_LIBRARIES}
    ${SDL_STATIC_LIBRARIES}
    ${SDL_IMAGE_STATIC_LIBRARIES}
    ${SDL_MIXER_STATIC_LIBRARIES}
    ${SDL_TTF_STATIC_LIBRARIES}
    ${Boost_LIBRARIES}
    ${MRuby_LIBRARIES}
    ${PCRE_STATIC_LIBRARIES}
    ${PNG_STATIC_LIBRARIES}
    lua
    intl
    )
  # For some unknown reason, nobody knows how to properly detect libintl?
  # However, as we already require Gettext anyway, linking libintl in directly
  # should be safe.
else()
  target_link_libraries(smc
    ${CEGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${FREEIMAGE_LIBRARIES}
    ${SDL_LIBRARIES}
    ${SDL_IMAGE_LIBRARIES}
    ${SDL_MIXER_LIBRARIES}
    ${SDL_TTF_LIBRARIES}
    ${Boost_LIBRARIES}
    ${MRuby_LIBRARIES}
    ${PCRE_LIBRARIES}
    ${PNG_LIBRARIES}
    lua
    )
endif()

# Installation instructions
install(TARGETS smc
  DESTINATION bin)
install(DIRECTORY "${SMC_SOURCE_DIR}/data/" # Note trailing slash for content copy
  DESTINATION share/smc)
install(DIRECTORY "${SMC_SOURCE_DIR}/src/scripting/mruby/" # Note trailing slash for content copy
  DESTINATION share/smc/scripting)
