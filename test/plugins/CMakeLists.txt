
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/target/dependency/include/safmq)


add_subdirectory (tasks)
add_subdirectory (hooks)
add_subdirectory (services)
add_subdirectory (MHiveStorage)


