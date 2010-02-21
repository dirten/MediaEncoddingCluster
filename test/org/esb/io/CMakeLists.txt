include_directories (${MEC_SOURCE_DIR}/src)

add_executable (TestFile TestFile.cpp)
add_executable (TestStringInputStream TestStringInputStream.cpp)
add_executable (TestStringOutputStream TestStringOutputStream.cpp)

target_link_libraries (TestFile io lang util)
target_link_libraries (TestStringInputStream io lang util)
target_link_libraries (TestStringOutputStream io lang util)

ADD_TEST(LibIOFile ${EXECUTABLE_OUTPUT_PATH}/TestFile)


