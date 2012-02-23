

add_library (encodingtask SHARED
  EncodingTask.cpp
  Packetizer.cpp
  StreamPacketizer.cpp
  ProcessUnitBuilder.cpp
  StreamProcessUnitBuilder.cpp
  ProcessUnitData.cpp
)

TARGET_LINK_LIBRARIES(encodingtask
  core
  partitionservice
)

SET_TARGET_PROPERTIES( encodingtask PROPERTIES PREFIX "")

INSTALL(TARGETS encodingtask
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
