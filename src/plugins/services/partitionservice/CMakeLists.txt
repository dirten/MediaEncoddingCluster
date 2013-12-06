


add_library (partitionservice SHARED
  Endpoint.cpp
  Partition.cpp
  Stream.cpp
  PartitionManager.cpp
  ProcessUnitCollector.cpp
 )

TARGET_LINK_LIBRARIES(partitionservice
  core
  ${Boost_LIBRARIES}
)

#SET_TARGET_PROPERTIES( partitionservice PROPERTIES PREFIX "")



INSTALL(TARGETS partitionservice
  RUNTIME DESTINATION ${LIB_INSTALL_DIR}
  LIBRARY DESTINATION ${LIB_INSTALL_DIR}
  #ARCHIVE DESTINATION plugins
)
