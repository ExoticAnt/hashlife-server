# CMakeList.txt : CMake project for hashlife-server, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("hashlife-server")

# Add source to this project's executable.
add_executable (hashlife-server "hashlife-server.cpp" "hashlife-server.h")

#library directories
include_directories(${CMAKE_SOURCE_DIR}/boost)

# TODO: Add tests and install targets if needed.
