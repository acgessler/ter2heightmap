cmake_minimum_required (VERSION 2.6)
project (ter2heightmap)

# The version number.
set (TER2HEIGHTMAP_VERSION_MAJOR 1)
set (TER2HEIGHTMAP_VERSION_MINOR 0)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/src/version.h.in"
  "${PROJECT_SOURCE_DIR}/src/version.h"
  )


add_executable(ter2heightmap src/main.cpp)

