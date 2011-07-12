
add_subdirectory (pkg)

#add_go_executable (mainrpc mainrpc.go )
add_go_executable (xml xml.go)
add_go_executable (node node.go)
add_go_executable (hive hive.go)
#add_go_executable (ffmpeg ffmpegtest.go)

#TARGET_LINK_GO_LIBRARIES(mainrpc hivenet)
  INSTALL(FILES
      ${CMAKE_CURRENT_BINARY_DIR}/xml
    DESTINATION ${BIN_INSTALL_DIR}
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ )

#INSTALL(TARGETS mainrpc
#    RUNTIME DESTINATION ${BIN_INSTALL_DIR}
#    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
#    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
#)


