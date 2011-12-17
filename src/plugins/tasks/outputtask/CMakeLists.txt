

add_library (outputtask SHARED
  OutputTask.cpp
)

TARGET_LINK_LIBRARIES(outputtask
  core
)

SET_TARGET_PROPERTIES( outputtask PROPERTIES PREFIX "")

INSTALL(TARGETS outputtask
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
