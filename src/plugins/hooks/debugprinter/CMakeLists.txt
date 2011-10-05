
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${LOG4CPLUS_DIR}/include/litesql)
include_directories (${FFMPEG_INCLUDE_DIR})



add_library (debugprinter SHARED
  DebugPrinter.cpp
)

TARGET_LINK_LIBRARIES(debugprinter
  core
)
SET_TARGET_PROPERTIES( debugprinter PROPERTIES PREFIX "")

INSTALL(TARGETS debugprinter
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
  #ARCHIVE DESTINATION plugins
)
