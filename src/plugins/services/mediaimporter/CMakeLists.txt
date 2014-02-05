
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})
include_directories (${LOG4CPLUS_DIR}/include/litesql)




add_library (mediaimporter SHARED
  FileImporter.cpp
 )

TARGET_LINK_LIBRARIES(mediaimporter
av
signal
core
)

SET_TARGET_PROPERTIES( mediaimporter PROPERTIES PREFIX "")



INSTALL(TARGETS mediaimporter
  RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR}
  LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
  #ARCHIVE DESTINATION plugins
)
