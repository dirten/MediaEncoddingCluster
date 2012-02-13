ADD_DEFINITIONS(-DCORE_BUILD_SHARED)

MESSAGE("CORE Build Type" ${CORE_LINK_TYPE})
MESSAGE("CoreHttp LinkLib"  ${POCO_LIB_NET})
MESSAGE("CoreHttp LinkLib"  ${POCO_LIB_NET_UTIL})
MESSAGE("CoreHttp LinkLib"  ${POCO_LIB_FOUNDATION})
MESSAGE("CoreHttp LinkLib"  ${POCO_LIB_XML})
include_directories (${BREAKPAD_INCLUDE_DIR})
LINK_DIRECTORIES(${BREAKPAD_LIB_DIR})

SET(core_http_source
  Server.cpp
  HTTPServerRequest.cpp
  HTTPServerResponse.cpp
  RootRequestHandler.cpp
)

add_library (core_http SHARED
  ${core_http_source}
)

TARGET_LINK_LIBRARIES(core_http
  ${POCO_LIB_NET_UTIL}
  ${POCO_LIB_NET}
  ${POCO_LIB_FOUNDATION}
  ${POCO_LIB_XML}
  util
  io
  hive
)
SET_TARGET_PROPERTIES( core_http PROPERTIES PREFIX "libmhive-")

INSTALL(TARGETS core_http
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
##    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
