include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MEC_SOURCE_DIR})
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)


add_executable (PartitionManagerTest PartitionManagerTest.cpp)

target_link_libraries (PartitionManagerTest partitionservice)



