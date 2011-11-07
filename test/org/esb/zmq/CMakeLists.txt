include_directories (${GLOOX_INCLUDE_DIR})


add_executable (ZmqTest ZmqTest.cpp)

target_link_libraries (ZmqTest ${ZMQ_LIBRARY} )
IF(WIN32)
	target_link_libraries (ZmqTest wsock32 Ws2_32)
ENDIF(WIN32)

IF(UNIX AND NOT APPLE)
	target_link_libraries (ZmqTest uuid pthread)
ENDIF(UNIX AND NOT APPLE)
