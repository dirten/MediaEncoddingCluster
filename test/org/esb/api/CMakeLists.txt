include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MEC_SOURCE_DIR})
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)


add_executable (ApiWebserverTest ApiWebserverTest.cpp)

target_link_libraries (ApiWebserverTest lang hive)

ADD_TEST(api.ApiWebserverTest ${EXECUTABLE_OUTPUT_PATH}/ApiWebserverTest)


