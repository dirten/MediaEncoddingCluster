

add_library (streamsource SHARED
  StreamSource.cpp
)

TARGET_LINK_LIBRARIES(streamsource
  core
)

SET_TARGET_PROPERTIES( streamsource PROPERTIES PREFIX "")

INSTALL(TARGETS streamsource
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
get_target_property(STREAMTASK_PLUGIN streamsource LOCATION)
ADD_DEFINITIONS(-DSTREAMTASK_PLUGIN="${STREAMTASK_PLUGIN}")
