
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
db
signal
core
hive
)

SET_TARGET_PROPERTIES( mediaimporter PROPERTIES PREFIX "")



INSTALL(TARGETS mediaimporter
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
  #ARCHIVE DESTINATION plugins
)
