include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)

add_executable (TestSqlConnection TestSqlConnection.cpp)

target_link_libraries (TestSqlConnection sql hive av io net signal esb lang util)
ADD_TEST(TestSqlConnection ${EXECUTABLE_OUTPUT_PATH}/TestSqlConnection )


