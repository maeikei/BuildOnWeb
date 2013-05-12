add_subdirectory(WebResources)

include_directories(app)
include_directories(${PROJECT_SOURCE_DIR}/httpframework)
link_directories(${CMAKE_BINARY_DIR}/3rdpart/boost/lib)
link_directories(${CMAKE_BINARY_DIR}/3rdpart/uri/lib)

aux_source_directory(app app_SRCS)


add_executable(cochub
  app/git_worker.hpp
  app/last_position.hpp
  app/utilities.hpp
  ${app_SRCS}
)

target_link_libraries( cochub httpframework lua leveldb)

if(NOT WIN32)
  target_link_libraries( cochub
    boost_iostreams
    boost_system
    boost_filesystem
    boost_regex
  )
endif(NOT WIN32)

if(APPLE)
  target_link_libraries( cochub
    c++
    c++abi
  )
endif(APPLE)

install(TARGETS cochub DESTINATION ${PROJECT_BINARY_DIR}/CocHubRoot)
