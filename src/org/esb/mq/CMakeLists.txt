


include_directories (${MEC_SOURCE_DIR}/target/dependency/include/safmq)

add_library (mq
      QueueManager.cpp
)

TARGET_LINK_LIBRARIES(mq
    ${Boost_LIBRARIES}
    lang
    safmq
    util
    io
    ws2_32
rpcrt4
)
IF(BUILD_SHARED_LIBS)
INSTALL(TARGETS mq
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
ENDIF(BUILD_SHARED_LIBS)

