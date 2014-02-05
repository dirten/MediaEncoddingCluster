

add_library (remote SHARED
  StartHandler.cpp
  StopHandler.cpp
  StatusHandler.cpp
  RemoteController.cpp
  NodeListHandler.cpp
  NodeDetailHandler.cpp
)

TARGET_LINK_LIBRARIES(remote
  core
  json
  grid
)
SET_TARGET_PROPERTIES( remote PROPERTIES PREFIX "")

INSTALL(TARGETS remote
  RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR}
  LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
)
