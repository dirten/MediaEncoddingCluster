
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})




add_library (auservice SHARED
  AutoUpdateService.cpp
 )

TARGET_LINK_LIBRARIES(auservice
  core
)

SET_TARGET_PROPERTIES( auservice PROPERTIES PREFIX "")


#installation currently disabled
INSTALL(TARGETS auservice
  RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR}
  LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
)
