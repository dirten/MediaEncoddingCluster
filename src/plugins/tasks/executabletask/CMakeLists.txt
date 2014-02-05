

add_library (executabletask SHARED
  ExecutableTask.cpp
)

TARGET_LINK_LIBRARIES(executabletask
  core
)

SET_TARGET_PROPERTIES( executabletask PROPERTIES PREFIX "")

INSTALL(TARGETS executabletask
  RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR}
  LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
)
