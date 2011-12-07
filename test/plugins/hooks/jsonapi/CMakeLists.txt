include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MEC_SOURCE_DIR})
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)


add_executable (PresetVerifierTest PresetVerifierTest.cpp)
add_executable (GraphVerifierTest GraphVerifierTest.cpp)

target_link_libraries (PresetVerifierTest jsonapi)
target_link_libraries (GraphVerifierTest jsonapi)

get_target_property(DOWNLOADTASK_PLUGIN downloadtask LOCATION)
ADD_DEFINITIONS(-DDOWNLOADTASK_PLUGIN="${DOWNLOADTASK_PLUGIN}")

get_target_property(ENCODINGTASK_PLUGIN encodingtask LOCATION)
ADD_DEFINITIONS(-DENCODINGTASK_PLUGIN="${ENCODINGTASK_PLUGIN}")

get_target_property(EXPORTTASK_PLUGIN exporttask LOCATION)
ADD_DEFINITIONS(-DEXPORTTASK_PLUGIN="${EXPORTTASK_PLUGIN}")

#ADD_TEST(core.PresetVerifierTest ${EXECUTABLE_OUTPUT_PATH}/PresetVerifierTest)


