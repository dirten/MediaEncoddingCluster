include_directories (${LOG4CPLUS_DIR}/include/safmq)
include_directories (${MEC_SOURCE_DIR}/target/dependency/include/safmq)

#add_executable (SimpleEnqueue SimpleEnqueue.cpp)
#target_link_libraries (SimpleEnqueue io)
#ADD_TEST(mq.SimpleEnqueue ${EXECUTABLE_OUTPUT_PATH}/SimpleEnqueue )

#add_executable (QueueOutputStreamTest QueueOutputStream.cpp)
#target_link_libraries (QueueOutputStreamTest io hive)
#ADD_TEST(mq.QueueOutputStreamTest ${EXECUTABLE_OUTPUT_PATH}/QueueOutputStreamTest)

#add_executable (QueueInputStreamTest QueueInputStream.cpp)
#target_link_libraries (QueueInputStreamTest io hive io)
#ADD_TEST(mq.QueueInputStreamTest ${EXECUTABLE_OUTPUT_PATH}/QueueInputStreamTest)

#add_executable (StartSafmqTest StartSafmqTest.cpp)
#target_link_libraries (StartSafmqTest io hive mq )
#ADD_TEST(mq.StartSafmqTest ${EXECUTABLE_OUTPUT_PATH}/StartSafmqTest)

add_executable (QueueManagerTest QueueManagerTest.cpp)
target_link_libraries (QueueManagerTest mq hive)
ADD_TEST(mq.QueueManagerTest ${EXECUTABLE_OUTPUT_PATH}/QueueManagerTest)

add_executable (QueueListenerTest QueueListenerTest.cpp)
target_link_libraries (QueueListenerTest io mq hive)

add_executable (SelectorTest SelectorTest.cpp)
target_link_libraries (SelectorTest mq hive)
ADD_TEST(mq.SelectorTest ${EXECUTABLE_OUTPUT_PATH}/SelectorTest)
