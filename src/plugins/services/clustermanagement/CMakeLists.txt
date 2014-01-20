
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})




add_library (clustermanagement SHARED
  ClusterService.cpp
 )

TARGET_LINK_LIBRARIES(clustermanagement
  signal
)

SET_TARGET_PROPERTIES( clustermanagement PROPERTIES PREFIX "")


#installation currently disabled
INSTALL(TARGETS clustermanagement
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
