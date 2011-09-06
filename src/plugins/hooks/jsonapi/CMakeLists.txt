
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${LOG4CPLUS_DIR}/include/litesql)

include_directories (${FFMPEG_INCLUDE_DIR})

    ADD_DEFINITIONS(-DJSONAPI_BUILD_SHARED)
    SET(JSONAPI_LINK_TYPE SHARED)


add_library (jsonapi ${JSONAPI_LINK_TYPE}
  ProcessHandler.cpp
  JSONHandler.cpp
)

TARGET_LINK_LIBRARIES(jsonapi
  core
  db
  json
  webservice
  av
  hive
)
SET_TARGET_PROPERTIES( jsonapi PROPERTIES PREFIX "")

INSTALL(TARGETS jsonapi
  LIBRARY DESTINATION plugins
  ARCHIVE DESTINATION plugins
)
