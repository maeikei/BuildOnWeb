add_subdirectory(WebResources)

include_directories(app)
include_directories(${PROJECT_SOURCE_DIR}/httpframework)
link_directories(${CMAKE_BINARY_DIR}/3rdpart/boost/lib)
link_directories(${CMAKE_BINARY_DIR}/3rdpart/uri/lib)

aux_source_directory(app app_SRCS)


add_executable(BuildOnWeb
  app/default_view.hpp
  app/directory_view.hpp
  app/git_worker.hpp
  app/last_position.hpp
  app/manual_view.hpp
  app/navi_view.hpp
  app/redirect_view.hpp
  app/repository_view.hpp
  app/socials_view.hpp
  app/source_view.hpp
  app/utilities.hpp
  ${app_SRCS}
)

target_link_libraries( BuildOnWeb httpframework lua leveldb)

if(NOT WIN32)
  target_link_libraries( BuildOnWeb
    boost_iostreams
    boost_system
    boost_filesystem
    boost_regex
  )
endif(NOT WIN32)

if(APPLE)
  target_link_libraries( BuildOnWeb
    c++
    c++abi
  )
endif(APPLE)

install(TARGETS BuildOnWeb DESTINATION ${PROJECT_BINARY_DIR}/InstallRoot)
