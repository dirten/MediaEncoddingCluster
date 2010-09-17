include_directories (${MEC_SOURCE_DIR}/src)

add_executable (TestStringUtil TestStringUtil.cpp)
add_executable (TestQueue TestQueue.cpp)
add_executable (TestBoostQueue TestBoostQueue.cpp)

target_link_libraries (TestStringUtil io util lang )
target_link_libraries (TestQueue lang util )
target_link_libraries (TestBoostQueue lang util )
IF(NOT WIN32 AND NOT APPLE)
target_link_libraries (TestBoostQueue rt)
ENDIF(NOT WIN32 AND NOT APPLE)
ADD_TEST(util.StringUtilTest ${EXECUTABLE_OUTPUT_PATH}/TestStringUtil )
ADD_TEST(util.QueueTest ${EXECUTABLE_OUTPUT_PATH}/TestQueue )
#ADD_TEST(TestBoostQueue ${EXECUTABLE_OUTPUT_PATH}/TestBoostQueue )


