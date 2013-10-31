include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MEC_SOURCE_DIR})
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)
include_directories (${MEC_SOURCE_DIR}/target/dependency/include/safmq)


add_executable (PluginRegistryTest PluginRegistryTest.cpp)
add_executable (PluginLoaderTest PluginLoaderTest.cpp)
add_executable (HookNotificationCenterTest HookNotificationCenterTest.cpp)
add_executable (BoostAnyCastTest BoostAnyCastTest.cpp)
add_executable (GraphTest GraphTest.cpp)
add_executable (GraphParserTest GraphParserTest.cpp)
add_executable (HTTPServerTest HTTPServerTest.cpp)
add_executable (PocoReactorServerTest PocoReactorServer.cpp)

target_link_libraries (PluginRegistryTest lang hive)
target_link_libraries (PluginLoaderTest io core)
target_link_libraries (HookNotificationCenterTest core)
target_link_libraries (BoostAnyCastTest core)
target_link_libraries (GraphTest encodingtask core)
target_link_libraries (GraphParserTest core)
target_link_libraries (HTTPServerTest core hive ${POCO_LIB_NET} ${POCO_LIB_FOUNDATION})
target_link_libraries (PocoReactorServerTest core ${POCO_LIB_NET} ${POCO_LIB_FOUNDATION})

ADD_TEST(core.PluginRegistryTest ${EXECUTABLE_OUTPUT_PATH}/PluginRegistryTest)
ADD_TEST(core.PluginLoaderTest ${EXECUTABLE_OUTPUT_PATH}/PluginLoaderTest)
ADD_TEST(core.HookNotificationCenterTest ${EXECUTABLE_OUTPUT_PATH}/HookNotificationCenterTest)
ADD_TEST(core.GraphParserTest ${EXECUTABLE_OUTPUT_PATH}/GraphParserTest ${MEC_SOURCE_DIR}/test-data/test.graph)
ADD_TEST(core.GraphTest.x264 ${EXECUTABLE_OUTPUT_PATH}/GraphTest ${MEC_SOURCE_DIR}/test-data/test.x264.graph ${PROJECT_SOURCE_DIR}/test.dvd)
ADD_TEST(core.GraphTest.vpx ${EXECUTABLE_OUTPUT_PATH}/GraphTest ${MEC_SOURCE_DIR}/test-data/test.vpx.graph ${PROJECT_SOURCE_DIR}/test.dvd)
#ADD_TEST(core.HTTPServerTest ${EXECUTABLE_OUTPUT_PATH}/HTTPServerTest)


add_library (plugin SHARED
  PluginRegistryTestPlugin.cpp
)
target_link_libraries (plugin core)
get_target_property(WEBSERVER_PLUGIN webservice LOCATION)
get_target_property(JSONSERVICE_PLUGIN jsonapi LOCATION)

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

get_target_property(OUTPUTTASK_PLUGIN outputtask LOCATION)
ADD_DEFINITIONS(-DOUTPUTTASK_PLUGIN="${OUTPUTTASK_PLUGIN}")

ADD_DEFINITIONS(-DWEBSERVER_PLUGIN="${WEBSERVER_PLUGIN}")
ADD_DEFINITIONS(-DJSONSERVICE_PLUGIN="${JSONSERVICE_PLUGIN}")
get_target_property(HTTPPULLTASK_PLUGIN httppullsource LOCATION)
ADD_DEFINITIONS(-DHTTPPULLTASK_PLUGIN="${HTTPPULLTASK_PLUGIN}")

get_target_property(MQSERVER_PLUGIN mqserver LOCATION)
ADD_DEFINITIONS(-DMQSERVER_PLUGIN="${MQSERVER_PLUGIN}")
