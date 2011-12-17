SET(SHARED_CORE on)

IF(SHARED_CORE)
    SET(CORE_LINK_TYPE SHARED)
    ADD_DEFINITIONS(-DCORE_BUILD_SHARED)
ELSE(SHARED_CORE)
    SET(CORE_LINK_TYPE STATIC)
ENDIF(SHARED_CORE)
MESSAGE("CORE Build Type" ${CORE_LINK_TYPE})

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
  Graph.cpp
  GraphParser.cpp
  GraphException.cpp
)

add_library (core ${CORE_LINK_TYPE}
  ${core_source}
)

TARGET_LINK_LIBRARIES(core 
  lang
  io
  hive
  db
  util
  ${POCO_LIB_NET}
  ${POCO_LIB_FOUNDATION}
)
SET_TARGET_PROPERTIES( core PROPERTIES PREFIX "libmhive-")

IF(SHARED_CORE)
INSTALL(TARGETS core
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
#    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
ENDIF(SHARED_CORE)
