

add_library (downloadtask SHARED
  DownloadTask.cpp
)

TARGET_LINK_LIBRARIES(downloadtask
  core
)

SET_TARGET_PROPERTIES( downloadtask PROPERTIES PREFIX "")

INSTALL(TARGETS downloadtask
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
