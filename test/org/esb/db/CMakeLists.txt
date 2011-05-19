include_directories (${LOG4CPLUS_DIR}/include/litesql)
include_directories (${LOG4CPLUS_DIR}/include/litesql/litesql)

add_executable (LiteSqlDatabaseCreateTest LiteSqlDatabaseCreateTest.cpp)
add_executable (ProfileGroupTest ProfileGroupTest.cpp)

target_link_libraries (LiteSqlDatabaseCreateTest hive)
target_link_libraries (ProfileGroupTest hive)


message("VARDB:" ${LD_VARNAME}=${LD_PATH})
ADD_TEST(LiteSqlDatabaseCreateTest ${EXECUTABLE_OUTPUT_PATH}/LiteSqlDatabaseCreateTest )
SET_TESTS_PROPERTIES(LiteSqlDatabaseCreateTest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

ADD_TEST(ProfileGroupTest ${EXECUTABLE_OUTPUT_PATH}/ProfileGroupTest )
SET_TESTS_PROPERTIES(ProfileGroupTest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")


