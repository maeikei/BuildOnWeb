project(BuildOnWeb)
cmake_minimum_required(VERSION 2.8)
include_directories(${PROJECT_SOURCE_DIR}/3rdpart_build/lua-5.2.1/src)

if(WIN32)
  include_directories(${CMAKE_BINARY_DIR}/3rdpart/include/boost-1_53)
else(WIN32)
  include_directories(${CMAKE_BINARY_DIR}/3rdpart/include)
endif(WIN32)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif(APPLE)

if(NOT WIN32)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif(NOT WIN32)


add_subdirectory(3rdpart_build)
add_subdirectory(server)
add_subdirectory(WebResources)
