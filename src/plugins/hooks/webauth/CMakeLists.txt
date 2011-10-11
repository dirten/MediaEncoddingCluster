

add_library (webauth SHARED
  UserHandler.cpp
)

TARGET_LINK_LIBRARIES(webauth
  core
  webservice
)
SET_TARGET_PROPERTIES( webauth PROPERTIES PREFIX "")

INSTALL(TARGETS webauth
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
