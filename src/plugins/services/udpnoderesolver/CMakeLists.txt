
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})
include_directories (${LOG4CPLUS_DIR}/include/litesql)




add_library (udpnoderesolver SHARED
  NodeResolver.cpp
  Service.cpp
 )

TARGET_LINK_LIBRARIES(udpnoderesolver
  core
)

SET_TARGET_PROPERTIES( udpnoderesolver PROPERTIES PREFIX "")


#installation currently disabled
INSTALL(TARGETS udpnoderesolver
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
