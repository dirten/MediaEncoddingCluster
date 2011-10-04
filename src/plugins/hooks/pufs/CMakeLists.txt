
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${LOG4CPLUS_DIR}/include/litesql)

include_directories (${FFMPEG_INCLUDE_DIR})

ADD_DEFINITIONS(-DPUFS_BUILD_SHARED)


add_library (pufs SHARED
  Pufs.cpp
)

TARGET_LINK_LIBRARIES(pufs
  core
  av
  webservice
)
SET_TARGET_PROPERTIES( pufs PROPERTIES PREFIX "")

INSTALL(TARGETS pufs
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
  #ARCHIVE DESTINATION plugins
)
