include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MEC_SOURCE_DIR})
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)


add_executable (PresetVerifierTest PresetVerifierTest.cpp)

target_link_libraries (PresetVerifierTest jsonapi)

ADD_TEST(core.PresetVerifierTest ${EXECUTABLE_OUTPUT_PATH}/PresetVerifierTest)


