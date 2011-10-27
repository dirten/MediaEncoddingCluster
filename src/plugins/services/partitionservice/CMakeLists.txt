


add_library (partitionservice SHARED
  PartitionManager.cpp
  ProcessUnitCollector.cpp
 )

TARGET_LINK_LIBRARIES(partitionservice
  core
)

SET_TARGET_PROPERTIES( partitionservice PROPERTIES PREFIX "")



INSTALL(TARGETS partitionservice
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
  #ARCHIVE DESTINATION plugins
)
