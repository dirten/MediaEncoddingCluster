


#include_directories (${MEC_SOURCE_DIR}/target/dependency/include/safmq)
include_directories (${SAFMQ_DIR}/include/safmq)
include_directories (${SAFMQ_DIR}/include/safmq/shared)
include_directories (${SAFMQ_DIR}/include/shared)
include_directories (${SAFMQ_DIR}/include/lib)
#LINK_DIRECTORIES(${SAFMQ_DIR}/lib)

add_library (mq SHARED
      QueueManager.cpp
      QueueConnection.cpp
      Selector.cpp
      QueueMessage.cpp
      ObjectMessage.cpp
      QueueException.cpp
      ../io/QueueOutputStream.cpp
      ../io/QueueInputStream.cpp

)

TARGET_LINK_LIBRARIES(mq
    safmq
    #${Boost_LIBRARIES}
    io
    util

)
IF(WIN32)
TARGET_LINK_LIBRARIES(mq
		ws2_32
		rpcrt4
		)
ENDIF(WIN32)


SET_TARGET_PROPERTIES( mq PROPERTIES PREFIX "libmhive-")

INSTALL(TARGETS mq
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})

