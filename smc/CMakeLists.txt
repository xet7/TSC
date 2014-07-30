cmake_minimum_required(VERSION 2.8)
enable_language(CXX)
project(SMC)

# Ensure our custom CMake modules get found
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Some CMake modules we use
include(ExternalProject)

########################################
# Version number
set(SMC_VERSION_MAJOR 2)
set(SMC_VERSION_MINOR 0)
set(SMC_VERSION_PATCH 0)

###############################################
# Flags & Options

option(ENABLE_MRUBY "Enable the MRuby scripting engine" ON)
set(FIXED_DATA_DIR "" CACHE FILEPATH "Enforce a static directory to look up graphics, music, etc. under rather than having SMC determine it dynamically.")

#FIXME: Actually use ENABLE_MRUBY

# Verbose compilation for better code quality
if(CMAKE_COMPILER_IS_GNUCC)
  add_definitions("-Wall")
endif()

# Extra flags for debugging SMC, additional to those
# already added by CMake itself when doing a debug
# build (e.g. -g is added for GCC by CMake).
if(${CMAKE_BUILD_TYPE} MATCHES Debug)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG=1")
endif()

execute_process(COMMAND git rev-parse --short HEAD
  OUTPUT_VARIABLE SMC_VERSION_GIT
  OUTPUT_STRIP_TRAILING_WHITESPACE)

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
    message(SEND_ERROR "Cannot find a rake executable")
  endif()
  if(GPERF_EXECUTABLE)
    message(STATUS "Found gperf: ${GPERF_EXECUTABLE}")
  else()
    message(SEND_ERROR "Cannot find a gperf executable")
  endif()
endif()

# Several libraries we depend on
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(CEGUI COMPONENTS OPENGL REQUIRED)
find_package(Gettext)
find_package(BISON REQUIRED)

if (WIN32)
  find_package(LibIntl REQUIRED)
  find_package(FreeImage REQUIRED)
  find_package(Boost 1.50.0
    COMPONENTS filesystem chrono thread_win32 system
    REQUIRED)

  # Boost-thread and CEGUI need to be explicitely told that they’re
  # linked in statically on Windows.
  add_definitions(-DBOOST_THREAD_USE_LIB)
else()
  set(Boost_USE_STATIC_LIBS OFF)
  find_package(DevIL REQUIRED)
  find_package(Boost 1.50.0
    COMPONENTS filesystem chrono thread system
    REQUIRED)
endif()

pkg_check_modules(PNG REQUIRED libpng)
pkg_check_modules(SDL REQUIRED sdl)
pkg_check_modules(SDL_IMAGE REQUIRED SDL_image)
pkg_check_modules(SDL_MIXER REQUIRED SDL_mixer)
pkg_check_modules(SDL_TTF REQUIRED SDL_ttf)
pkg_check_modules(PCRE REQUIRED libpcre)
pkg_check_modules(LibXmlPP REQUIRED libxml++-2.6)

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
#  ${IL_INCLUDE_DIR}
  ${PCRE_INCLUDE_DIRS}
  ${LibXmlPP_INCLUDE_DIRS}
  )

if (WIN32)
  include_directories(${FreeImage_INCLUDE_DIRS})
else()
  include_directories(${IL_INCLUDE_DIR})
endif()

###############################################
# mruby

if (ENABLE_MRUBY)
  # Tell CMake how to compile mruby
  set(MRUBY_TOPDIR ${SMC_SOURCE_DIR}/../mruby)
  set(MRUBY_ROOT_DIR ${MRUBY_TOPDIR}/mruby)
  set(MRUBY_MGEMS_DIR ${MRUBY_TOPDIR}/mgems)

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

###############################################
# Configuration header

configure_file(${SMC_SOURCE_DIR}/config.hpp.in
  ${SMC_BINARY_DIR}/config.hpp)
include_directories(${SMC_BINARY_DIR})


########################################
# Credits file

set_source_files_properties(${SMC_SOURCE_DIR}/src/credits.cpp
  PROPERTIES GENERATED true)
add_custom_command(OUTPUT ${SMC_SOURCE_DIR}/src/credits.cpp
  COMMAND rake src/credits.cpp
  DEPENDS ${SMC_SOURCE_DIR}/docs/authors.txt)

###############################################
# Source files

# For now, just assume ALL cpp files to be requisites
# of SMC.
file(GLOB_RECURSE smc_sources
  "src/*.cpp"
  "src/*.hpp"
  )

###############################################
# Targets

# Now add our build targets and their dependencies.
add_executable(smc ${smc_sources} ${SMC_SOURCE_DIR}/src/credits.cpp)
if (ENABLE_MRUBY)
  add_dependencies(smc mruby)
endif()
# TODO: Make the mruby config automatically include the correct path to the PCRE library
if (WIN32)
  target_link_libraries(smc
    ${CEGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${FreeImage_LIBRARIES}
    ${SDL_STATIC_LIBRARIES}
    ${SDL_IMAGE_STATIC_LIBRARIES}
    ${SDL_MIXER_STATIC_LIBRARIES}
    ${SDL_TTF_STATIC_LIBRARIES}
    ${Boost_LIBRARIES}
    ${MRuby_LIBRARIES}
    ${PCRE_STATIC_LIBRARIES}
    ${LibXmlPP_STATIC_LIBRARIES}
    ${PNG_STATIC_LIBRARIES}
    intl
    ws2_32
    )
  # For some unknown reason, nobody knows how to properly detect libintl?
  # However, as we already require Gettext anyway, linking libintl in directly
  # should be safe.
else()
  target_link_libraries(smc
    ${CEGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${IL_LIBRARIES}
    ${ILU_LIBRARIES}
    ${SDL_LIBRARIES}
    ${SDL_IMAGE_LIBRARIES}
    ${SDL_MIXER_LIBRARIES}
    ${SDL_TTF_LIBRARIES}
    ${Boost_LIBRARIES}
    ${MRuby_LIBRARIES}
    ${PCRE_LIBRARIES}
    ${LibXmlPP_LIBRARIES}
    ${PNG_LIBRARIES}
    )
endif()

# Installation instructions
install(TARGETS smc
  DESTINATION bin
  COMPONENT base)
install(DIRECTORY "${SMC_SOURCE_DIR}/data/campaigns/" # Note trailing slash for content copy
  DESTINATION share/smc/campaigns
  COMPONENT campaigns)
install(DIRECTORY "${SMC_SOURCE_DIR}/data/editor/"
  DESTINATION share/smc/editor
  COMPONENT base)
install(DIRECTORY "${SMC_SOURCE_DIR}/data/gui/"
  DESTINATION share/smc/gui
  COMPONENT base)
install(DIRECTORY "${SMC_SOURCE_DIR}/data/icon/"
  DESTINATION share/smc/icon
  COMPONENT base)
install(DIRECTORY "${SMC_SOURCE_DIR}/data/levels/"
  DESTINATION share/smc/levels
  COMPONENT levels)
install(DIRECTORY "${SMC_SOURCE_DIR}/data/music/"
  DESTINATION share/smc/music
  COMPONENT music)
install(DIRECTORY "${SMC_SOURCE_DIR}/data/pixmaps/"
  DESTINATION share/smc/pixmaps
  COMPONENT base)
install(DIRECTORY "${SMC_SOURCE_DIR}/data/schema/"
  DESTINATION share/smc/schema
  COMPONENT base)
install(DIRECTORY "${SMC_SOURCE_DIR}/data/scripting/"
  DESTINATION share/smc/scripting
  COMPONENT base)
install(DIRECTORY "${SMC_SOURCE_DIR}/data/sounds/"
  DESTINATION share/smc/sounds
  COMPONENT sounds)
install(DIRECTORY "${SMC_SOURCE_DIR}/data/translations/"
  DESTINATION share/smc/translations
  COMPONENT base)
install(DIRECTORY "${SMC_SOURCE_DIR}/data/worlds/"
  DESTINATION share/smc/worlds
  COMPONENT worlds)

########################################
# Packager

# Main info
set(CPACK_PACKAGE_NAME "SMC")
set(CPACK_PACKAGE_VENDOR "SMC Team")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SMC - 2-dimensional platform game")
set(CPACK_PACKAGE_VERSION_MAJOR ${SMC_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${SMC_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${SMC_VERSION_PATCH})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "SMC")
set(CPACK_PACKAGE_EXECUTABLES smc "SMC")

# Nicer names
set(CPACK_COMPONENT_BASE_DISPLAY_NAME "Basic application components (required)")
set(CPACK_COMPONENT_CAMPAIGNS_DISPLAY_NAME "Core campaigns")
set(CPACK_COMPONENT_LEVELS_DISPLAY_NAME "Core levels")
set(CPACK_COMPONENT_MUSIC_DISPLAY_NAME "Music files")
set(CPACK_COMPONENT_SOUNDS_DISPLAY_NAME "Sound files")
set(CPACK_COMPONENT_WORLDS_DISPLAY_NAME "Core worlds")

# Descriptions
set(CPACK_COMPONENT_BASE_DESCRIPTION "The smc program itself and its core requirements.")
set(CPACK_COMPONENT_CAMPAIGNS_DESCRIPTION "Standard campaigns included with SMC.")
set(CPACK_COMPONENT_LEVELS_DESCRIPTION "Standard levels included with SMC.")
set(CPACK_COMPONENT_MUSIC_DESCRIPTION "Background music files. No background music without this.")
set(CPACK_COMPONENT_SOUNDS_DESCRIPTION "Sound files. No sounds without this.")
set(CPACK_COMPONENT_WORLDS_DESCRIPTION "Standard worlds included with SMC.")

# Interdependencies
set(CPACK_COMPONENT_CAMPAIGNS_DEPENDS worlds)
set(CPACK_COMPONENT_WORLDS_DEPENDS levels)

# Required marks
set(CPACK_COMPONENT_BASE_REQUIRED ON)

# Misc
set(CPACK_RESOURCE_FILE_LICENSE "${SMC_SOURCE_DIR}/docs/license.txt")
set(CPACK_CREATE_DESKTOP_LINKS smc)
set(CPACK_NSIS_MODIFY_PATH "ON") # Required for desktop shortcuts
set(CPACK_NSIS_MUI_ICON "${SMC_SOURCE_DIR}/extras/icons/idr_main.ico") # Installer icon
set(CPACK_NSIS_MUI_UNIICON "${SMC_SOURCE_DIR}/extras/icons/idr_main.ico") # Uninstaller icon
set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\smc.exe") # Icon for system control panel
set(CPACK_PACKAGE_ICON "${SMC_SOURCE_DIR}/extras/icons/idr_main.ico") # Installer window bar icon

include(CPack)
