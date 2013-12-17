
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})




add_library (writerservice SHARED
  WriterService.cpp
  Writer.cpp
 )

TARGET_LINK_LIBRARIES(writerservice
  core
)

SET_TARGET_PROPERTIES( writerservice PROPERTIES PREFIX "")


#installation currently disabled
INSTALL(TARGETS writerservice
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
