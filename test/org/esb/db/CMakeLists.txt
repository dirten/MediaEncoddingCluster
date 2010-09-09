include_directories (${LOG4CPLUS_DIR}/include/litesql)
include_directories (${LOG4CPLUS_DIR}/include/litesql/litesql)

add_executable (LiteSqlDatabaseCreateTest LiteSqlDatabaseCreateTest.cpp)
add_executable (ProfileGroupTest ProfileGroupTest.cpp)

target_link_libraries (LiteSqlDatabaseCreateTest hive)
target_link_libraries (ProfileGroupTest hive)

ADD_TEST(db.LiteSqlDatabaseCreateTest ${EXECUTABLE_OUTPUT_PATH}/LiteSqlDatabaseCreateTest )
ADD_TEST(db.ProfileGroupTest ${EXECUTABLE_OUTPUT_PATH}/ProfileGroupTest )


