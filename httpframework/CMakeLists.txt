include_directories(.)
link_directories(${CMAKE_BINARY_DIR}/3rdpart/boost/lib)
link_directories(${CMAKE_BINARY_DIR}/3rdpart/uri/lib)

aux_source_directory(. framework_SRCS)


add_library(httpframework 
  ./connection.hpp
  ./header.hpp
  ./mime_types.hpp
  ./reply.hpp
  ./reply_view.hpp
  ./request.hpp
  ./request_handler.hpp
  ./request_parser.hpp
  ./resource.hpp
  ./server.hpp
  ${framework_SRCS}
)
