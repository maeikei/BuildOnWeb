project(BuildOnWeb)
cmake_minimum_required(VERSION 2.8)
include_directories(${PROJECT_SOURCE_DIR}/3rdpart_build/lua-5.2.1/src)
include_directories(${PROJECT_SOURCE_DIR}/3rdpart_build/leveldb-1.9.0/include)


if(WIN32)
  include_directories(${CMAKE_BINARY_DIR}/3rdpart/include/boost-1_53)
else(WIN32)
  include_directories(${CMAKE_BINARY_DIR}/3rdpart/boost/include)
endif(WIN32)
include_directories(${CMAKE_BINARY_DIR}/3rdpart/uri/include)
include_directories(${CMAKE_BINARY_DIR}/3rdpart/cpp-netlib/include)


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
add_subdirectory(httpframework)
add_subdirectory(BuildOnWeb)
add_subdirectory(cochub)
