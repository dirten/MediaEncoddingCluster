

add_library (remote SHARED
  StartHandler.cpp
  StopHandler.cpp
  StatusHandler.cpp
  RemoteController.cpp
  NodeListHandler.cpp
)

TARGET_LINK_LIBRARIES(remote
  core
  json
  grid
)
SET_TARGET_PROPERTIES( remote PROPERTIES PREFIX "")

INSTALL(TARGETS remote
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
