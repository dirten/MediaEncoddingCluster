
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${LOG4CPLUS_DIR}/include/litesql)

include_directories (${FFMPEG_INCLUDE_DIR})

    ADD_DEFINITIONS(-DJSONAPI_BUILD_SHARED)


add_library (jsonapi SHARED
#  ProcessHandler.cpp
  JSONHandler.cpp
  FileHandler.cpp
  PresetVerifier.cpp
  ConfigHandler.cpp
  JobHandler.cpp
  GraphHandler.cpp
)

TARGET_LINK_LIBRARIES(jsonapi
  core
  json
  webservice
  av
)
SET_TARGET_PROPERTIES( jsonapi PROPERTIES PREFIX "")

INSTALL(TARGETS jsonapi
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
  #ARCHIVE DESTINATION plugins
)
