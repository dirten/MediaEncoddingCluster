
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})
include_directories (${LOG4CPLUS_DIR}/include/litesql)




add_library (mediaexporter SHARED
  FileExporter.cpp
  ExportScanner.cpp
 )

TARGET_LINK_LIBRARIES(mediaexporter
av
hive
core
)

SET_TARGET_PROPERTIES( mediaexporter PROPERTIES PREFIX "")



INSTALL(TARGETS mediaexporter
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
  #ARCHIVE DESTINATION plugins
)
