

add_library (httppullsource SHARED
  HTTPPullSource.cpp
)

TARGET_LINK_LIBRARIES(httppullsource
  core
)

SET_TARGET_PROPERTIES( httppullsource PROPERTIES PREFIX "")

INSTALL(TARGETS httppullsource
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
get_target_property(HTTPPULLTASK_PLUGIN httppullsource LOCATION)
ADD_DEFINITIONS(-DHTTPPULLTASK_PLUGIN="${HTTPPULLTASK_PLUGIN}")
