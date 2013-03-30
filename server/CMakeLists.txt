link_directories(${CMAKE_BINARY_DIR}/3rdpart/lib)
aux_source_directory(framework framework_SRCS)
add_executable(BuildOnWeb
  ${framework_SRCS}
)
target_link_libraries( BuildOnWeb lua)
target_link_libraries( BuildOnWeb
  boost_iostreams
  boost_system
)
if(WIN32)
else(WIN32)
  target_link_libraries( BuildOnWeb
    c++
    c++abi
  )
endif(WIN32)

install(TARGETS BuildOnWeb DESTINATION ${PROJECT_BINARY_DIR}/InstallRoot/bin)
