include_directories (${GLOOX_INCLUDE_DIR})


add_executable (ZmqTest ZmqTest.cpp)
add_executable (hwserver HWServer.cpp)
add_executable (hwclient HWClient.cpp)

target_link_libraries (ZmqTest ${ZMQ_LIBRARY} )
target_link_libraries (hwserver ${ZMQ_LIBRARY} )
target_link_libraries (hwclient ${ZMQ_LIBRARY} )


IF(WIN32)
	target_link_libraries (ZmqTest wsock32 Ws2_32 rpcrt4)
	target_link_libraries (hwserver wsock32 Ws2_32 rpcrt4)
	target_link_libraries (hwclient wsock32 Ws2_32 rpcrt4)
ENDIF(WIN32)

IF(UNIX AND NOT APPLE)
	target_link_libraries (ZmqTest uuid pthread)
	target_link_libraries (hwserver uuid pthread)
	target_link_libraries (hwclient uuid pthread)
ENDIF(UNIX AND NOT APPLE)
