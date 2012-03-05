
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})
include_directories (${LOG4CPLUS_DIR}/include/litesql)
include_directories (${MEC_SOURCE_DIR}/target/dependency/include/safmq)




add_library (mqserver SHARED
    Service.cpp
 )

TARGET_LINK_LIBRARIES(mqserver
  mq
  core
)

SET_TARGET_PROPERTIES( mqserver PROPERTIES PREFIX "")



INSTALL(TARGETS mqserver
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
  #ARCHIVE DESTINATION plugins
)
