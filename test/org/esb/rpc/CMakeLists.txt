include_directories (${LOG4CPLUS_DIR}/include/litesql)
include_directories (${LOG4CPLUS_DIR}/include/litesql/litesql)

add_executable (RpcServerTest RpcServerTest.cpp)
add_executable (RpcClientTest RpcClientTest.cpp)
add_executable (RpcRegisterNode RpcRegisterNode.cpp)


target_link_libraries (RpcServerTest rpc core)
target_link_libraries (RpcClientTest rpc protobuf protobuf-lite core)
target_link_libraries (RpcRegisterNode rpc core)



