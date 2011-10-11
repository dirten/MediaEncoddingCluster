INCLUDE_DIRECTORIES (${MEC_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES (${MEC_SOURCE_DIR}/target/dependency/include)
INCLUDE_DIRECTORIES (${MEC_SOURCE_DIR}/target/dependency/include/litesql)


add_subdirectory (services)
add_subdirectory (hooks)
