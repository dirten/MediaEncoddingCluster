
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})




add_library (jobexecutor SHARED
  Service.cpp
 )

TARGET_LINK_LIBRARIES(jobexecutor
  core
)

SET_TARGET_PROPERTIES( jobexecutor PROPERTIES PREFIX "")


#installation currently disabled
INSTALL(TARGETS jobexecutor
  RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR}
  LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
)
