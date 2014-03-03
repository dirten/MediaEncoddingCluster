

#add_subdirectory (http)

include_directories (${BREAKPAD_INCLUDE_DIR})
LINK_DIRECTORIES(${BREAKPAD_LIB_DIR})

SET(core_source
  PluginRegistry.cpp
  PluginLoader.cpp
  AppContext.cpp
  PluginContext.cpp
  HookNotificationCenter.cpp
  Application.cpp
  Task.cpp
  TaskException.cpp
  Graph.cpp
  GraphParser.cpp
  GraphException.cpp
  WebHookHandlerFactory.cpp
  WebHookProxy.cpp
  Version.cpp
  #http/Server.cpp
  #http/HTTPServerRequest.cpp
  #http/HTTPServerResponse.cpp
  #http/RootRequestHandler.cpp
  ../hive/job/ProcessUnit.cpp
  ../hive/DatabaseService.cpp
  ../hive/HiveClient.cpp
  ../hive/HiveException.cpp
  ../hive/PresetReaderJson.cpp
  ../config/config.cpp
  ../hive/Environment.cpp

)

SET(core_http_source
  http/Server.cpp
  http/HTTPServerRequest.cpp
  http/HTTPServerResponse.cpp
  http/RootRequestHandler.cpp
)

add_library (core SHARED
  ${core_source}
  ${core_http_source}
)

MESSAGE("PocoLinkLibarayUtil"  ${POCO_LIB_NET_UTIL})

TARGET_LINK_LIBRARIES(core 
  #${POCO_LIB_FOUNDATION}
  #json
  #db
  #net
  #grid

  #signal
  #lang
  #util
  #io
  #${POCO_LIB_XML}
  #${Boost_LIBRARIES}
  #lang
  #io
#  iphlpapi
#  hive
  #db
  #util
  #core_http
#  ${POCO_LIB_NET}
#  ${POCO_LIB_NET_UTIL}
)

if(MINGW)
  TARGET_LINK_LIBRARIES(core
    #ws2_32
    #wsock32
    #gdi32
  )
  TARGET_LINK_LIBRARIES(core wsock32 ws2_32 advapi32 iphlpapi)

ENDIF(MINGW)

SET_TARGET_PROPERTIES( core PROPERTIES PREFIX "libmhive-")
INSTALL(TARGETS core
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
    #ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)


