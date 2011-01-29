
add_subdirectory (pkg)

add_go_executable (mainrpc mainrpc.go )
add_go_executable (xml xml.go)
add_go_executable (node node.go)
#add_go_executable (ffmpeg ffmpegtest.go)

TARGET_LINK_GO_LIBRARIES(mainrpc
    hivenet
)

#INSTALL(TARGETS mainrpc
#    RUNTIME DESTINATION ${BIN_INSTALL_DIR}
#    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
#    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
#)


