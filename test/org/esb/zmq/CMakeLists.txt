include_directories (${GLOOX_INCLUDE_DIR})


add_executable (ZmqTest ZmqTest.cpp)

target_link_libraries (ZmqTest ${ZMQ_LIBRARY} )

