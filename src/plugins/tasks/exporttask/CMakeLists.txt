

add_library (exporttask SHARED
  ExportTask.cpp
  ProcessUnitData.cpp
)

TARGET_LINK_LIBRARIES(exporttask
  core
)

SET_TARGET_PROPERTIES( exporttask PROPERTIES PREFIX "")

INSTALL(TARGETS exporttask
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
