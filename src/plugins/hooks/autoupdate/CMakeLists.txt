

add_library (autoupdate SHARED
  AutoUpdateHandler.cpp
  UpdateDownloadHandler.cpp
  UpdateUploadHandler.cpp
  UpdateInstallHandler.cpp
)

TARGET_LINK_LIBRARIES(autoupdate
  core
  json
)
SET_TARGET_PROPERTIES( autoupdate PROPERTIES PREFIX "")

INSTALL(TARGETS autoupdate
  RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR}
  LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
)
