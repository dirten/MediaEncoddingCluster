include_directories (${MEC_SOURCE_DIR}/src)
#include_directories (/home/jhoelscher/google-breakpad-read-only/src)
#LINK_DIRECTORIES    (/home/jhoelscher/breakpad/lib)

add_executable (TestThread TestThread.cpp)
#add_executable (TestException TestException.cpp)
add_executable (TestStackDumper TestStackDumper.cpp)
#add_executable (TestLibCurl TestLibCurl.cpp)

target_link_libraries (TestThread lang)
#target_link_libraries (TestException lang)
target_link_libraries (TestStackDumper lang util)
#target_link_libraries (TestLibCurl lang util)
if(NOT WIN32)
  target_link_libraries (TestThread pthread)
ENDIF(NOT WIN32)
ADD_TEST(LibLang ${EXECUTABLE_OUTPUT_PATH}/TestThread )
#ADD_TEST(TestException ${EXECUTABLE_OUTPUT_PATH}/TestException )
ADD_TEST(TestStackDumper ${EXECUTABLE_OUTPUT_PATH}/TestStackDumper )
#ADD_TEST(TestLibCurl ${EXECUTABLE_OUTPUT_PATH}/TestLibCurl )


