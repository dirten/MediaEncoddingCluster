

add_library (downloadtask SHARED
  DownloadTask.cpp
)

TARGET_LINK_LIBRARIES(downloadtask
  core
  ${POCO_LIB_NET}
  ${POCO_LIB_FOUNDATION}
)

SET_TARGET_PROPERTIES( downloadtask PROPERTIES PREFIX "")

INSTALL(TARGETS downloadtask
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
