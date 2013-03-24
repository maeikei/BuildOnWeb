project(BuildOnWeb)
cmake_minimum_required(VERSION 2.8)
include_directories(${PROJECT_SOURCE_DIR}/3rdpart_build/lua-5.2.1/src)
include_directories(${CMAKE_BINARY_DIR}/3rdpart/include/boost-1_53)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_subdirectory(3rdpart_build)
add_subdirectory(server)
