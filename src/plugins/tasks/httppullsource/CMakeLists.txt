

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
