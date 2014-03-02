
SET( JSONAPI_SOVERSION 0.1.0 )

include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${LOG4CPLUS_DIR}/include/litesql)

include_directories (${FFMPEG_INCLUDE_DIR})

ADD_DEFINITIONS(-DJSONAPI_BUILD_SHARED)


add_library (jsonapi SHARED
#  ProcessHandler.cpp
#  JSONResult.cpp
#  JSONHandler.cpp
#  FileHandler.cpp
  PresetVerifier.cpp
#  ConfigHandler.cpp
#  JobHandler.cpp
#  GraphHandler.cpp
#API Handling of all Graph/Flow Requests
  graph/GraphListHandler.cpp
  graph/GraphLoadHandler.cpp
  graph/GraphUpdateHandler.cpp
  graph/GraphCreateHandler.cpp
  graph/GraphDeleteHandler.cpp
  graph/GraphSubmitHandler.cpp
  GraphVerifier.cpp
  #GraphTaskCreator.cpp

#API Handling of all File/Media Requests
  fileapi/FileHandler.cpp
  fileapi/MediaHandler.cpp

#API Handling of all Format/Codec Requests
  av/FormatListHandler.cpp
  av/FormatLoadHandler.cpp
  av/CodecListHandler.cpp
  av/CodecInfoHandler.cpp

#API Handling of all Profile Requests
  profile/ProfileListHandler.cpp
  profile/ProfileLoadHandler.cpp
  profile/ProfileCreateHandler.cpp
  profile/ProfileDeleteHandler.cpp

#API Handling of all Encoding Requests
  encoding/EncodingListHandler.cpp
  encoding/EncodingLoadHandler.cpp
  encoding/EncodingCreateHandler.cpp
  encoding/EncodingStopHandler.cpp
  encoding/EncodingDeleteHandler.cpp
  encoding/EncodingRestartHandler.cpp
  encoding/EncodingStatusHandler.cpp
  encoding/EncodingUploadHandler.cpp

#API Handling the Watchfolder Requests
  watchfolder/WatchfolderCreateHandler.cpp

)
# @TODO: this is only a hack for the linker, this is needed to be fixed asap
IF(WIN32)
  #SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wl,--allow-multiple-definition" )
  SET( CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS}  -Wl,--allow-multiple-definition" )
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(jsonapi
  streamsource
  core
  json
  av
#  av
  ${POCO_LIB_NET}
  ${POCO_LIB_FOUNDATION}

)
SET_TARGET_PROPERTIES( jsonapi PROPERTIES PREFIX "")

#IF( WIN32 )
#    SET_TARGET_PROPERTIES( jsonapi PROPERTIES
#        OUTPUT_NAME "jsonapi-${JSONAPI_SOVERSION}"
#        VERSION  ${JSONAPI_SOVERSION} )
#ELSE()
#    SET_TARGET_PROPERTIES( jsonapi PROPERTIES
#        VERSION  "${JSONAPI_SOVERSION}"
#        SOVERSION "${JSONAPI_SOVERSION}" )
#ENDIF()

INSTALL(TARGETS jsonapi
  RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR}
  LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
  #ARCHIVE DESTINATION plugins
)
