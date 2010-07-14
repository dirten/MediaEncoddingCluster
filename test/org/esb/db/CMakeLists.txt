include_directories (${LOG4CPLUS_DIR}/include/litesql)
include_directories (${LOG4CPLUS_DIR}/include/litesql/litesql)

add_executable (LiteSqlDatabaseCreateTest LiteSqlDatabaseCreateTest.cpp)

target_link_libraries (LiteSqlDatabaseCreateTest hive)
ADD_TEST(LiteSqlDatabaseCreateTest ${EXECUTABLE_OUTPUT_PATH}/LiteSqlDatabaseCreateTest )


