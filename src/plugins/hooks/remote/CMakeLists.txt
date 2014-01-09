

add_library (remote SHARED
  StartHandler.cpp
  StopHandler.cpp
  RemoteController.cpp
)

TARGET_LINK_LIBRARIES(remote
  core
  json
)
SET_TARGET_PROPERTIES( remote PROPERTIES PREFIX "")

INSTALL(TARGETS remote
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
