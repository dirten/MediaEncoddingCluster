ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=64)

SET(PROTOC_CMD protoc)
IF(WIN32)
    SET(PROTOC_CMD ${PROTOC_CMD}.exe)
ENDIF(WIN32)

MACRO (GENERATE_PROTO infile)
ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${infile}.pb.cc
	           COMMAND ${MEC_SOURCE_DIR}/target/dependency/bin/${PROTOC_CMD}
                   ARGS --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}
                	--proto_path=${CMAKE_CURRENT_SOURCE_DIR}
                        ${CMAKE_CURRENT_SOURCE_DIR}/${infile}.proto
		   MAIN_DEPENDENCY
		        ${CMAKE_CURRENT_SOURCE_DIR}/${infile}.proto
)
ENDMACRO (GENERATE_PROTO)

GENERATE_PROTO(rpc)

include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MEC_SOURCE_DIR}/target/dependency/include/)
LINK_DIRECTORIES(${MEC_SOURCE_DIR}/target/dependency/lib)

add_library (rpc
    Channel.cpp
    Controller.cpp
    Server.cpp
    Client.cpp
    rpc.pb.cc
    services/ProcessUnitServiceImpl.cpp
    services/ProcessServiceImpl.cpp
    services/NodeServiceImpl.cpp
    services/ApiServiceImpl.cpp
)

TARGET_LINK_LIBRARIES(rpc ${PROTOBUF_LIBRARY} net hive)

