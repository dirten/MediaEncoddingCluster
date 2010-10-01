
include_directories (${MEC_SOURCE_DIR})
add_executable (DatabaseBootstrapTest db/DatabaseBootstrapTest.cpp)

target_link_libraries (DatabaseBootstrapTest hive )
ADD_TEST(db.DatabaseBootstrapTest ${EXECUTABLE_OUTPUT_PATH}/DatabaseBootstrapTest)
set_tests_properties(db.DatabaseBootstrapTest PROPERTIES ENVIRONMENT "MYSQLD_BIN=${MEC_SOURCE_DIR}/target/dependency/bin/mysqld")

add_subdirectory (lang)
add_subdirectory (io)
add_subdirectory (util)
add_subdirectory (db)
add_subdirectory (net)
add_subdirectory (av)
add_subdirectory (hive)
add_subdirectory (web)
#add_subdirectory (orm)
add_subdirectory (mq)


