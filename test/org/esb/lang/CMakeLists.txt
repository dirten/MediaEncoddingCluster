include_directories (${MEC_SOURCE_DIR}/src)

add_executable (TestThread TestThread.cpp)

target_link_libraries (TestThread lang)
if(NOT WIN32)
  target_link_libraries (TestThread pthread)
ENDIF(NOTWIN32)
ADD_TEST(LibLang ${EXECUTABLE_OUTPUT_PATH}/TestThread )


