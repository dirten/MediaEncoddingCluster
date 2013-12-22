

add_library (autoupdate SHARED
  AutoUpdateHandler.cpp
  UpdateDownloadHandler.cpp
)

TARGET_LINK_LIBRARIES(autoupdate
  core
  json
)
SET_TARGET_PROPERTIES( autoupdate PROPERTIES PREFIX "")

INSTALL(TARGETS autoupdate
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
