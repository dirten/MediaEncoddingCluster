include_directories (${MEC_SOURCE_DIR}/src)
#include_directories (/home/jhoelscher/google-breakpad-read-only/src)
#LINK_DIRECTORIES    (/home/jhoelscher/breakpad/lib)

add_executable (TestThread TestThread.cpp)
add_executable (TestException TestException.cpp)
#add_executable (TestStackDumper TestStackDumper.cpp)
#add_executable (TestSystemCpuInfo TestSystemCpuInfo.cpp)
add_executable (ProcessTest ProcessTest.cpp)
add_executable (ProcessTestExecutable ProcessTestExecutable.cpp)
add_executable (SharedObjectLoaderTest SharedObjectLoaderTest.cpp)
#add_executable (ObjectTest ObjectTest.cpp)

#target_link_libraries (TestSystemCpuInfo io lang util net )
target_link_libraries (TestThread lang util)
target_link_libraries (TestException lang)
#target_link_libraries (TestStackDumper lang util)
target_link_libraries (ProcessTest lang io util)
target_link_libraries (ProcessTestExecutable lang util)
target_link_libraries (SharedObjectLoaderTest lang util io)
#target_link_libraries (ObjectTest lang)

if(NOT WIN32)
  target_link_libraries (TestThread pthread)
ENDIF(NOT WIN32)
#target_link_libraries (ObjectTest lang)
message("VAR" ${LD_VARNAME}=${LD_PATH})
ADD_TEST(lang.ThreadTest ${EXECUTABLE_OUTPUT_PATH}/TestThread )
SET_TESTS_PROPERTIES(lang.ThreadTest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

#ADD_TEST(TestSystemCpuInfo ${EXECUTABLE_OUTPUT_PATH}/TestSystemCpuInfo )
ADD_TEST(lang.ExceptionTest ${EXECUTABLE_OUTPUT_PATH}/TestException )
SET_TESTS_PROPERTIES(lang.ExceptionTest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

#ADD_TEST(lang.StackDumperTest ${EXECUTABLE_OUTPUT_PATH}/TestStackDumper )
#SET_TESTS_PROPERTIES(lang.StackDumperTest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

ADD_TEST(lang.ProcessTest ${EXECUTABLE_OUTPUT_PATH}/ProcessTest )
SET_TESTS_PROPERTIES(lang.ProcessTest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")
#ADD_TEST(TestLibCurl ${EXECUTABLE_OUTPUT_PATH}/TestLibCurl )
#ADD_TEST(ObjectTest ${EXECUTABLE_OUTPUT_PATH}/ObjectTest )
ADD_TEST(lang.SharedObjectLoaderTest ${EXECUTABLE_OUTPUT_PATH}/SharedObjectLoaderTest )



add_library (shared_object SHARED
  SharedObject.cpp
)
SET_TARGET_PROPERTIES( shared_object PROPERTIES PREFIX "lib")