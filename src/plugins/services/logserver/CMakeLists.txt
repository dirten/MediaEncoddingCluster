
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})
include_directories (${LOG4CPLUS_DIR}/include/litesql)





add_library (logserver SHARED
    Service.cpp
 )

TARGET_LINK_LIBRARIES(logserver
  core
)

SET_TARGET_PROPERTIES( logserver PROPERTIES PREFIX "")



INSTALL(TARGETS logserver
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
  #ARCHIVE DESTINATION plugins
)
