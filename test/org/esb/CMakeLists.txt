
include_directories (${MEC_SOURCE_DIR})
add_executable (DatabaseBootstrapTest db/DatabaseBootstrapTest.cpp)

target_link_libraries (DatabaseBootstrapTest db ${MYSQL_LIBRARY} esb)
ADD_TEST(db.DatabaseBootstrapTest ${EXECUTABLE_OUTPUT_PATH}/DatabaseBootstrapTest)


add_subdirectory (lang)
add_subdirectory (io)
add_subdirectory (util)
add_subdirectory (db)
add_subdirectory (net)
add_subdirectory (av)
add_subdirectory (hive)
add_subdirectory (web)
add_subdirectory (orm)
add_subdirectory (mq)


