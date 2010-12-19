


include_directories (${MEC_SOURCE_DIR}/target/dependency/include/safmq)

add_library (mq
      QueueManager.cpp
      QueueConnection.cpp
      QueueMessage.cpp
      QueueException.cpp
)
IF(WIN32)
	set(EXTRA_LIBS 
		ws2_32
		rpcrt4
	)
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(mq
    ${Boost_LIBRARIES}
    lang
    safmq
    util
    io
    ${EXTRA_LIBS}
    hive
)
IF(BUILD_SHARED_LIBS)
INSTALL(TARGETS mq
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
ENDIF(BUILD_SHARED_LIBS)

