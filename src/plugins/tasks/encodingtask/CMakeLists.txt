

add_library (encodingtask SHARED
  EncodingTask.cpp
)

TARGET_LINK_LIBRARIES(encodingtask
  core
)

SET_TARGET_PROPERTIES( encodingtask PROPERTIES PREFIX "")

INSTALL(TARGETS encodingtask
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
