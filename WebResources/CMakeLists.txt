# install html file.
FILE(GLOB HTML_FILES ./*.html)
install(FILES ${HTML_FILES} DESTINATION ${PROJECT_BINARY_DIR}/InstallRoot)
install(DIRECTORY ./css DESTINATION ${PROJECT_BINARY_DIR}/InstallRoot)
