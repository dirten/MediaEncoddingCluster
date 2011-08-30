include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MEC_SOURCE_DIR})
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)


add_executable (PluginRegistryTest PluginRegistryTest.cpp)
add_executable (PluginLoaderTest PluginLoaderTest.cpp)

target_link_libraries (PluginRegistryTest lang hive)
target_link_libraries (PluginLoaderTest core)

ADD_TEST(core.PluginRegistryTest ${EXECUTABLE_OUTPUT_PATH}/PluginRegistryTest)
ADD_TEST(core.PluginLoaderTest ${EXECUTABLE_OUTPUT_PATH}/PluginLoaderTest)


add_library (plugin SHARED
  PluginRegistryTestPlugin.cpp
)
target_link_libraries (plugin core)
get_target_property(WEBSERVER_PLUGIN webserver LOCATION)
get_target_property(JSONSERVICE_PLUGIN jsonservice LOCATION)

ADD_DEFINITIONS(-DWEBSERVER_PLUGIN="${WEBSERVER_PLUGIN}")
ADD_DEFINITIONS(-DJSONSERVICE_PLUGIN="${JSONSERVICE_PLUGIN}")
