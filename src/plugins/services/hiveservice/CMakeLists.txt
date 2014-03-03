  
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})
include_directories (${LOG4CPLUS_DIR}/include/litesql)
include_directories (${MEC_SOURCE_DIR}/target/dependency/include/safmq)
include_directories (${SAFMQ_DIR}/include/lib)
include_directories (${SAFMQ_DIR}/include/shared)





add_library (hiveservice SHARED
  HiveListener.cpp
  ProtocolServer.cpp
  proto/QueueNameHandler.cpp
  #proto/PartitionHandler.cpp
 )

TARGET_LINK_LIBRARIES(hiveservice

  core

)

SET_TARGET_PROPERTIES( hiveservice PROPERTIES PREFIX "")



INSTALL(TARGETS hiveservice
  RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR}
  LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
  #ARCHIVE DESTINATION plugins
)
