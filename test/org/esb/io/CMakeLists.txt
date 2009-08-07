include_directories (${MEC_SOURCE_DIR}/src)

add_executable (TestFile TestFile.cpp)

target_link_libraries (TestFile io lang)

ADD_TEST(LibIOFile ${EXECUTABLE_OUTPUT_PATH}/TestFile)


