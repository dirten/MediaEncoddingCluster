


include_directories (${MEC_SOURCE_DIR}/target/dependency/include)

add_library (grid
  GridNode.cpp
  GridNodeServer.cpp
  NodeResolver.cpp
  protocol/ProtocolDispatcher.cpp
  protocol/ServerHandler.cpp
  command/ServerCommand.cpp
)
TARGET_LINK_LIBRARIES(grid
    ${Boost_LIBRARIES}
    lang
    util
)
IF(BUILD_SHARED_LIBS)
INSTALL(TARGETS grid
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
ENDIF(BUILD_SHARED_LIBS)
add_executable (gridconsole   GridNodeConsole.cpp)
add_executable (gridservice   GridService.cpp)


TARGET_LINK_LIBRARIES(gridconsole
    ${Boost_LIBRARIES}
    lang
    util
    net
    grid
)
TARGET_LINK_LIBRARIES(gridservice
    rpc
)
INSTALL(TARGETS gridservice
    RUNTIME DESTINATION ${BIN_INSTALL_DIR}
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
