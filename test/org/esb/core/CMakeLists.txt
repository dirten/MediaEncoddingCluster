include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MEC_SOURCE_DIR})
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)


add_executable (PluginRegistryTest PluginRegistryTest.cpp)
add_executable (PluginLoaderTest PluginLoaderTest.cpp)
add_executable (HookNotificationCenterTest HookNotificationCenterTest.cpp)

target_link_libraries (PluginRegistryTest lang hive)
target_link_libraries (PluginLoaderTest io core)
target_link_libraries (HookNotificationCenterTest core)

ADD_TEST(core.PluginRegistryTest ${EXECUTABLE_OUTPUT_PATH}/PluginRegistryTest)
ADD_TEST(core.PluginLoaderTest ${EXECUTABLE_OUTPUT_PATH}/PluginLoaderTest)
ADD_TEST(core.HookNotificationCenterTest ${EXECUTABLE_OUTPUT_PATH}/HookNotificationCenterTest)


add_library (plugin SHARED
  PluginRegistryTestPlugin.cpp
)
target_link_libraries (plugin core)
get_target_property(WEBSERVER_PLUGIN webservice LOCATION)
get_target_property(JSONSERVICE_PLUGIN jsonapi LOCATION)

ADD_DEFINITIONS(-DWEBSERVER_PLUGIN="${WEBSERVER_PLUGIN}")
ADD_DEFINITIONS(-DJSONSERVICE_PLUGIN="${JSONSERVICE_PLUGIN}")
