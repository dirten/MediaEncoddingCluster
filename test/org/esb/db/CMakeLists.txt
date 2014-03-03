include_directories (${LOG4CPLUS_DIR}/include/litesql)
include_directories (${LOG4CPLUS_DIR}/include/litesql/litesql)

add_executable (DatabaseBootstrapTest DatabaseBootstrapTest.cpp)
add_executable (LiteSqlDatabaseCreateTest LiteSqlDatabaseCreateTest.cpp)
add_executable (ProfileGroupTest ProfileGroupTest.cpp)

target_link_libraries (LiteSqlDatabaseCreateTest core)
target_link_libraries (ProfileGroupTest core)
target_link_libraries (DatabaseBootstrapTest core )


message("VARDB:" ${LD_VARNAME}=${LD_PATH})
ADD_TEST(LiteSqlDatabaseCreateTest ${EXECUTABLE_OUTPUT_PATH}/LiteSqlDatabaseCreateTest )
SET_TESTS_PROPERTIES(LiteSqlDatabaseCreateTest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

ADD_TEST(db.ProfileGroupTest ${EXECUTABLE_OUTPUT_PATH}/ProfileGroupTest )
SET_TESTS_PROPERTIES(db.ProfileGroupTest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

ADD_TEST(db.DatabaseBootstrapTest ${EXECUTABLE_OUTPUT_PATH}/DatabaseBootstrapTest)
set_tests_properties(db.DatabaseBootstrapTest PROPERTIES ENVIRONMENT "MYSQLD_BIN=${MEC_SOURCE_DIR}/target/dependency/bin/mysqld")

