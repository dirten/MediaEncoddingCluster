
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})




add_library (mhive_storage SHARED
  services/engines/Simple.cpp
  services/StorageService.cpp
  services/engines/redundant_engine/RedundantEngine.cpp
  services/engines/redundant_engine/DLMBoostMutex.cpp
  services/engines/redundant_engine/DLMHost.cpp

  web/LockHook.cpp
  web/EnqueHandler.cpp
)

TARGET_LINK_LIBRARIES(mhive_storage
  core
  mongoclient

)

SET_TARGET_PROPERTIES( mhive_storage PROPERTIES PREFIX "")



INSTALL(TARGETS mhive_storage
  RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR}
  LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
)
