cmake_minimum_required(VERSION 2.8)
enable_language(CXX)
project(SMC)

# Ensure our custom CMake modules get found
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Configuration options for the library finding
# functions.
set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_STATIC_RUNTIME OFF)

# Check for the actual libraries.
find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(CEGUI COMPONENTS NULL OPENGL REQUIRED)
find_package(Boost 1.50.0
  COMPONENTS filesystem thread system
  REQUIRED)
find_package(Gettext)

# Add any definitiony required by libraries
add_definitions(
  ${PNG_DEFINITIONS}
  )

# Add all our libraries to our -I-nclude path
include_directories(
  ${OPENGL_INCLUDE_DIR}
  ${PNG_INCLUDE_DIRS}
  ${SDL_INCLUDE_DIR}
  ${SDL_IMAGE_INCLUDE_DIRS}
  ${SDL_MIXER_INCLUDE_DIRS}
  ${SDL_TTF_INCLUDE_DIRS}
  ${CEGUI_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  )

# Recurse for Lua and place the resulting
# liblua.a next to us.
add_subdirectory(lua ${SMC_BINARY_DIR}/lua)
include_directories(${SMC_SOURCE_DIR}/lua/lua-5.2.1/src)

# For now, just assume ALL cpp files to be requisites
# of SMC. It would be WAY better to split this up into
# smaller pieces using recursive cmake without globbing
# (this way you have to manually rerun CMake each time
# you add a new source file).
file(GLOB_RECURSE smc_sources
  "src/*.cpp"
  "src/*.hpp"
  "src/*.h"
  )

# Now add our build targets and their dependencies.
add_executable(smc ${smc_sources})
target_link_libraries(smc
  ${OPENGL_LIBRARIES}
  ${PNG_LIBRARIES}
  ${SDL_LIBRARY}
  ${SDL_IMAGE_LIBRARIES}
  ${SDL_MIXER_LIBRARIES}
  ${SDL_TTF_LIBRARIES}
  ${CEGUI_LIBRARY}
  ${Boost_LIBRARIES}
  lua
  )
