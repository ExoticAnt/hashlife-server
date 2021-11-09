# CMakeList.txt : CMake project for hashlife-server, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

#debug on-off
option(USE_DEBUG "Enter debug mode" OFF)


if (USE_DEBUG)
  add_definitions(-DDEBUG)
endif()

project ("hashlife-server")

# Add source to this project's executable.
add_executable (hashlife-server "hashlife-server.cpp" "hashlife-server.h")

#library directories
include_directories(${CMAKE_SOURCE_DIR}/boost_1_77_0)

# TODO: Add tests and install targets if needed.
