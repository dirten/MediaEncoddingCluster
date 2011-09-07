
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})


ADD_DEFINITIONS(-DWEBSERVICE_BUILD_SHARED)
SET(WEBSERVICE_LINK_TYPE SHARED)

add_library (webservice ${WEBSERVICE_LINK_TYPE}
    mongoose.c
    ApiWebServer.cpp
    ServiceRequest.cpp
    ServiceResponse.cpp
 )

TARGET_LINK_LIBRARIES(webservice
  util
  core
)

SET_TARGET_PROPERTIES( webservice PROPERTIES PREFIX "")



INSTALL(TARGETS webservice
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
  #ARCHIVE DESTINATION plugins
)
