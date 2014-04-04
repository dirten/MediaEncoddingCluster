include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MEC_SOURCE_DIR})
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)


add_executable (DLMBoostMutexTest DLMBoostMutextTest.cpp)
target_link_libraries (DLMBoostMutexTest mhive_storage)

get_target_property(STORAGE_PLUGIN mhive_storage LOCATION)
ADD_DEFINITIONS(-DSTORAGE_PLUGIN="${STORAGE_PLUGIN}")

add_executable (SimpleStorageTest SimpleStorageTest.cpp)
target_link_libraries (SimpleStorageTest core)




