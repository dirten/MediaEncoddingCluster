
include_directories (${MEC_SOURCE_DIR})
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})

add_library (json SHARED
    JSONChildren.cpp
    JSONDebug.cpp
    JSONIterators.cpp
    JSONMemory.cpp
    JSONNode.cpp
    JSONNode_Mutex.cpp
    JSONStream.cpp
    JSONValidator.cpp
    JSONWorker.cpp
    JSONWriter.cpp
    JSON_Base64.cpp
    internalJSONNode.cpp
    libjson.cpp
    JSONResult.cpp
)

TARGET_LINK_LIBRARIES(json
  lang
)

  INSTALL(TARGETS json
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})

