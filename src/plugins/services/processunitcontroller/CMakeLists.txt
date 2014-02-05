
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})
include_directories (${LOG4CPLUS_DIR}/include/litesql)





add_library (processunitcontroller SHARED
  Packetizer.cpp
  ProcessUnitBuilder.cpp
  ProcessUnitController.cpp
  PartitionManager.cpp
 )

TARGET_LINK_LIBRARIES(processunitcontroller
av
hive
core

)

SET_TARGET_PROPERTIES( processunitcontroller PROPERTIES PREFIX "")



INSTALL(TARGETS processunitcontroller
  RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR}
  LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
  #ARCHIVE DESTINATION plugins
)
