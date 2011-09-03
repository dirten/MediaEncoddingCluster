include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MEC_SOURCE_DIR})
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)


add_executable (ApiWebserverTest ApiWebserverTest.cpp)
add_executable (JSONHandlerTest JSONHandlerTest.cpp)

target_link_libraries (ApiWebserverTest lang hive)
target_link_libraries (JSONHandlerTest lang core)

ADD_TEST(api.ApiWebserverTest ${EXECUTABLE_OUTPUT_PATH}/ApiWebserverTest)
ADD_TEST(api.JSONHandlerTest ${EXECUTABLE_OUTPUT_PATH}/JSONHandlerTest)


get_target_property(WEBSERVER_PLUGIN webserver LOCATION)
ADD_DEFINITIONS(-DWEBSERVER_PLUGIN="${WEBSERVER_PLUGIN}")

get_target_property(JSONSERVICE_PLUGIN jsonservice LOCATION)
ADD_DEFINITIONS(-DJSONSERVICE_PLUGIN="${JSONSERVICE_PLUGIN}")

