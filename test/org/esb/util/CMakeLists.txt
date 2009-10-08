include_directories (${MEC_SOURCE_DIR}/src)

add_executable (TestStringUtil TestStringUtil.cpp)
add_executable (TestQueue TestQueue.cpp)

target_link_libraries (TestStringUtil io util lang )
target_link_libraries (TestQueue lang util )

ADD_TEST(TestStringUtil ${EXECUTABLE_OUTPUT_PATH}/TestStringUtil )
ADD_TEST(TestQueue ${EXECUTABLE_OUTPUT_PATH}/TestQueue )


