include_directories (${MEC_SOURCE_DIR}/target/dependency/include/safmq)
include_directories (${AMQ_DIR}/include/activemq-cpp-3.8.1)


add_library (encodingtask SHARED
  EncodingTask.cpp
  Packetizer.cpp
  StreamPacketizer.cpp
  ProcessUnitBuilder.cpp
  StreamProcessUnitBuilder.cpp
  ProcessUnitData.cpp
  ProcessUnitList.cpp
)

TARGET_LINK_LIBRARIES(encodingtask
  core
  partitionservice
  mq
  activemq-cpp
)

SET_TARGET_PROPERTIES( encodingtask PROPERTIES PREFIX "")

INSTALL(TARGETS encodingtask
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
