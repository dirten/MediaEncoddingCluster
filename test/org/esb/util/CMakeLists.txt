include_directories (${MEC_SOURCE_DIR}/src)

add_executable (TestStringUtil TestStringUtil.cpp)

target_link_libraries (TestStringUtil util io)
ADD_TEST(TestStringUtil ${EXECUTABLE_OUTPUT_PATH}/TestStringUtil )


