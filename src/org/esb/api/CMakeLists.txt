
include_directories (${MEC_SOURCE_DIR})
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})
SET(SHARED_API On)
SET(BUILD_CORE On)

IF(SHARED_API)
    ADD_DEFINITIONS(-DAPI_BUILD_SHARED)
    SET(API_LINK_TYPE SHARED)
ELSE(SHARED_API)
    SET(API_LINK_TYPE STATIC)
ENDIF(SHARED_API)

add_library (api ${API_LINK_TYPE}
    mongoose.c
    MHiveServerConnection.cpp
    MediaFile.cpp
    JsonServer.cpp
    JsonEncoding.cpp
    JsonProfileHandler.cpp
    JsonEncodingHandler.cpp
    JsonStatisticsHandler.cpp
    JsonMediaHandler.cpp
    JsonSystemHandler.cpp
 )
IF(BUILD_CORE)

add_library (webserver ${API_LINK_TYPE}
    mongoose.c
    ApiWebServer.cpp
    ServiceRequest.cpp
    ServiceResponse.cpp
 )

add_library (jsonservice ${API_LINK_TYPE}
  ProcessHandler.cpp
  JSONHandler.cpp
)

ENDIF(BUILD_CORE)


TARGET_LINK_LIBRARIES(api
    av
    json
    db
    util
    rpc
    hive
)
IF(BUILD_CORE)
TARGET_LINK_LIBRARIES(jsonservice
  core
  db
  json
  webserver
)
TARGET_LINK_LIBRARIES(webserver
  util
  core
)
SET_TARGET_PROPERTIES( webserver PROPERTIES PREFIX "libmhive-")
SET_TARGET_PROPERTIES( jsonservice PROPERTIES PREFIX "libmhive-")

ENDIF(BUILD_CORE)

SET_TARGET_PROPERTIES( api PROPERTIES PREFIX "libmhive-")

IF(SHARED_API)
  INSTALL(TARGETS api
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
  IF(BUILD_CORE)
  INSTALL(TARGETS webserver
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
  INSTALL(TARGETS jsonservice
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
  ENDIF(BUILD_CORE)

ENDIF(SHARED_API)