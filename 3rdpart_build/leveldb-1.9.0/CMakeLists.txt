include_directories(./)
add_definitions(-DLEVELDB_PLATFORM_POSIX)


if(APPLE)
  add_definitions(-DOS_MACOSX)
endif(APPLE)

aux_source_directory(db db_SRCS)
set( SRC
  ${db_SRCS}
)
add_library(leveldb ${SRC})
