include_directories (${LOG4CPLUS_DIR}/include/litesql)
include_directories (${LOG4CPLUS_DIR}/include/litesql/litesql)

add_executable (OrmDatabaseCreateTest OrmDatabaseCreateTest.cpp)

target_link_libraries (OrmDatabaseCreateTest hive orm)
ADD_TEST(OrmDatabaseCreateTest ${EXECUTABLE_OUTPUT_PATH}/OrmDatabaseCreateTest )


