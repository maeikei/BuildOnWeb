include_directories(./)
add_definitions(-DLEVELDB_PLATFORM_POSIX)


if(APPLE)
  add_definitions(-DOS_MACOSX)
endif(APPLE)

aux_source_directory(db db_SRCS)
aux_source_directory(port port_SRCS)
aux_source_directory(util util_SRCS)
aux_source_directory(table table_SRCS)


set( SRC
  ${db_SRCS}
  ${port_SRCS}
  ${util_SRCS}
  ${table_SRCS}
)
add_library(leveldb ${SRC})
