include_directories (${MEC_SOURCE_DIR}/src)

add_executable (TestTcpSocket TestTcpSocket.cpp)

target_link_libraries (TestTcpSocket lang net)
ADD_TEST(TestTcpSocket ${EXECUTABLE_OUTPUT_PATH}/TestTcpSocket )


