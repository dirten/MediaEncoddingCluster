include_directories (${MEC_SOURCE_DIR}/src)

add_executable (TestThread TestThread.cpp)
add_executable (TestException TestException.cpp)

target_link_libraries (TestThread lang)
target_link_libraries (TestException lang)
if(NOT WIN32)
  target_link_libraries (TestThread pthread)
ENDIF(NOT WIN32)
ADD_TEST(LibLang ${EXECUTABLE_OUTPUT_PATH}/TestThread )
ADD_TEST(TestException ${EXECUTABLE_OUTPUT_PATH}/TestException )


