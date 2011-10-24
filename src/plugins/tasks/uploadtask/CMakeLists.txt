

add_library (uploadtask SHARED
  UploadTask.cpp
)

TARGET_LINK_LIBRARIES(uploadtask
  core
)

SET_TARGET_PROPERTIES( uploadtask PROPERTIES PREFIX "")

INSTALL(TARGETS uploadtask
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
