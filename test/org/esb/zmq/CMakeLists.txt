include_directories (${GLOOX_INCLUDE_DIR})


add_executable (ZmqTest ZmqTest.cpp)

target_link_libraries (ZmqTest ${ZMQ_LIBRARY} )

IF(UNIX AND NOT APPLE)
	target_link_libraries (ZmqTest uuid)
ENDIF(UNIX AND NOT APPLE)
