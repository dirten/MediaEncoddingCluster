
include_directories (${MEC_SOURCE_DIR})
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})

add_library (api
    MHiveServerConnection.cpp
    MediaFile.cpp
    JsonServer.cpp
    JsonEncoding.cpp
    JsonProfileHandler.cpp
 )
TARGET_LINK_LIBRARIES(api
    av
    rpc
    json
    db
)

IF(BUILD_SHARED_LIBS)
  INSTALL(TARGETS api
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
ENDIF(BUILD_SHARED_LIBS)