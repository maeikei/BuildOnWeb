include_directories(app)
link_directories(${CMAKE_BINARY_DIR}/3rdpart/boost/lib)
link_directories(${CMAKE_BINARY_DIR}/3rdpart/uri/lib)

#aux_source_directory(app app_SRCS)



add_executable(BuildOnWeb2
#  app/template.hpp
#  app/env.hpp
#  app/navi_view.hpp
#  app/source_view.hpp
#  app/directory_view.hpp
#  app/manual_view.hpp
#  app/source_update.hpp
#  app/last_position.hpp
#  app/redirect_view.hpp
#  app/socials_view.hpp
#  ${app_SRCS}
  main.cpp
)

target_link_libraries( BuildOnWeb2 lua leveldb)

if(NOT WIN32)
  target_link_libraries( BuildOnWeb2
    boost_iostreams
    boost_system
    boost_filesystem
    boost_regex
  )
endif(NOT WIN32)

if(APPLE)
  target_link_libraries( BuildOnWeb2
    c++
    c++abi
  )
endif(APPLE)

install(TARGETS BuildOnWeb2 DESTINATION ${PROJECT_BINARY_DIR}/InstallRoot)
