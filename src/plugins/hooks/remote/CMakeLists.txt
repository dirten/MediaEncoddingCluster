

add_library (remote SHARED
  Start.cpp
  StopHandler.cpp
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
