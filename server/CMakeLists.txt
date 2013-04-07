include_directories(app)
include_directories(framework)
link_directories(${CMAKE_BINARY_DIR}/3rdpart/lib)

aux_source_directory(framework framework_SRCS)
aux_source_directory(app app_SRCS)



add_executable(BuildOnWeb
  framework/connection.hpp
  framework/header.hpp
  framework/mime_types.hpp
  framework/reply.hpp
  framework/request.hpp
  framework/request_handler.hpp
  framework/request_parser.hpp
  framework/server.hpp
  ${framework_SRCS}

  app/template.hpp
  app/env.hpp
  app/list_dir.hpp
  app/source_update.hpp
  ${app_SRCS}
)

target_link_libraries( BuildOnWeb lua)

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
