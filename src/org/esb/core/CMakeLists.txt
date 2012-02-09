

add_subdirectory (http)

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
)

add_library (core SHARED
  ${core_source}
)

TARGET_LINK_LIBRARIES(core 
  lang
  io
  hive
  db
  util
  #${POCO_LIB_NET}
  #${POCO_LIB_FOUNDATION}
  core_http
)

SET_TARGET_PROPERTIES( core PROPERTIES PREFIX "libmhive-")
INSTALL(TARGETS core
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
#    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)


