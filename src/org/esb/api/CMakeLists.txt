
include_directories (${MEC_SOURCE_DIR})
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})
SET(SHARED_API Off)

IF(SHARED_API)
    ADD_DEFINITIONS(-DAPI_BUILD_SHARED)
    SET(API_LINK_TYPE SHARED)
ELSE(SHARED_API)
    SET(API_LINK_TYPE STATIC)
ENDIF(SHARED_API)

add_library (api ${API_LINK_TYPE}
    mongoose.c
    ApiWebServer.cpp
    ProcessHandler.cpp
    MHiveServerConnection.cpp
    MediaFile.cpp
    JsonServer.cpp
    JsonEncoding.cpp
    JsonProfileHandler.cpp
    JsonEncodingHandler.cpp
    JsonStatisticsHandler.cpp
 )
TARGET_LINK_LIBRARIES(api
    av
    rpc
    json
    db
)

SET_TARGET_PROPERTIES( api PROPERTIES PREFIX "libmhive-")

#IF(BUILD_SHARED_LIBS)
  INSTALL(TARGETS api
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
#ENDIF(BUILD_SHARED_LIBS)