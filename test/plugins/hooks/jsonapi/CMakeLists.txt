include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MEC_SOURCE_DIR})
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)


add_executable (PresetVerifierTest PresetVerifierTest.cpp)
add_executable (GraphVerifierTest GraphVerifierTest.cpp)
add_executable (GraphTaskCreatorTest GraphTaskCreatorTest.cpp)
add_executable (BaseJSONApiTest BaseJSONApiTest.cpp)
add_executable (PocoRegExpTest PocoRegExpTest.cpp)

target_link_libraries (PresetVerifierTest jsonapi)
target_link_libraries (GraphVerifierTest jsonapi)
target_link_libraries (GraphTaskCreatorTest jsonapi)
target_link_libraries (BaseJSONApiTest core hive)
target_link_libraries (PocoRegExpTest core )

ADD_TEST(hooks.BaseJSONApiTest ${EXECUTABLE_OUTPUT_PATH}/BaseJSONApiTest)

get_target_property(DOWNLOADTASK_PLUGIN downloadtask LOCATION)
ADD_DEFINITIONS(-DDOWNLOADTASK_PLUGIN="${DOWNLOADTASK_PLUGIN}")

get_target_property(ENCODINGTASK_PLUGIN encodingtask LOCATION)
ADD_DEFINITIONS(-DENCODINGTASK_PLUGIN="${ENCODINGTASK_PLUGIN}")

get_target_property(EXPORTTASK_PLUGIN exporttask LOCATION)
ADD_DEFINITIONS(-DEXPORTTASK_PLUGIN="${EXPORTTASK_PLUGIN}")

get_target_property(EXECUTABLETASK_PLUGIN executabletask LOCATION)
ADD_DEFINITIONS(-DEXECUTABLETASK_PLUGIN="${EXECUTABLETASK_PLUGIN}")

get_target_property(UPLOADTASK_PLUGIN uploadtask LOCATION)
ADD_DEFINITIONS(-DUPLOADTASK_PLUGIN="${UPLOADTASK_PLUGIN}")

get_target_property(JSONAPI_PLUGIN jsonapi LOCATION)
ADD_DEFINITIONS(-DJSONAPI_PLUGIN="${JSONAPI_PLUGIN}")

#ADD_TEST(core.PresetVerifierTest ${EXECUTABLE_OUTPUT_PATH}/PresetVerifierTest)


