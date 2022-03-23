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
#add_executable (hashlife-server "hashlife-server.cpp" "hashlife-server.h" "main.cpp")
add_executable (hashlife-server "hashlife-server.cpp" "hashlife-server.h" "simul_proc.cpp" "simul_proc.h" "simple_implementation.h" "simple_implementation.cpp" "output_conversion.h")
#add_executable (hashlife-server "main.cpp" "simple_implementation.h" "simple_implementation.cpp")


#library directories
include_directories(${CMAKE_SOURCE_DIR}/boost_1_77_0)
link_directories(${CMAKE_SOURCE_DIR}/boost_1_77_0/bin/libs)

target_link_libraries(hashlife-server ${CMAKE_SOURCE_DIR}/boost_1_77_0/bin/libs/libboost_container-vc142-mt-gd-x32-1_77.lib)

# TODO: Add tests and install targets if needed.
enable_testing()
add_executable (SITest "SITest.cpp" "simple_implementation.h" "simple_implementation.cpp" "simul_proc.cpp" "simul_proc.h" "output_conversion.h")
add_test(NAME Simple_Implementation COMMAND SITest)