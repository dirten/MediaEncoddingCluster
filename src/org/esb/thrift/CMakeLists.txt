ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=64)

SET(THRIFT_CMD thrift)
IF(WIN32)
    SET(THRIFTCMD_CMD ${THRIFTCMD_CMD}.exe)
ENDIF(WIN32)

#MACRO (GENERATE_PROTO infile)
MESSAGE("hey")
ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/proto_types.cpp
             COMMAND ${MEC_SOURCE_DIR}/build/build/bin/${THRIFT_CMD}
                   ARGS --gen cpp
                        --out ${CMAKE_CURRENT_SOURCE_DIR}
                        ${CMAKE_CURRENT_SOURCE_DIR}/proto.idl
       MAIN_DEPENDENCY
            ${CMAKE_CURRENT_SOURCE_DIR}/proto.idl
)
#ENDMACRO (GENERATE_PROTO)

#GENERATE_PROTO(proto)
#ADD_LIBRARY(mhive-thrift proto_constants.cpp proto_types.cpp Calculator.cpp)
LINK_DIRECTORIES(${THRIFT_DIR}/lib)
TARGET_LINK_LIBRARIES(mhive-thrift thrift)
#include_directories (${MEC_SOURCE_DIR}/src)
#include_directories (${MEC_SOURCE_DIR}/target/dependency/include/)
#LINK_DIRECTORIES(${MEC_SOURCE_DIR}/target/dependency/lib)
#
#add_library (rpc
#    Channel.cpp
#    Controller.cpp
#    Server.cpp
#    Client.cpp
#    rpc.pb.cc
#    services/ProcessUnitServiceImpl.cpp
#    services/ProcessServiceImpl.cpp
#    services/NodeServiceImpl.cpp
#    services/ApiServiceImpl.cpp
#)

#TARGET_LINK_LIBRARIES(rpc ${PROTOBUF_LIBRARY} net hive)

