INCLUDE_DIRECTORIES (${FFMPEG_INCLUDE_DIR})
INCLUDE_DIRECTORIES (${MEC_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES (${MEC_SOURCE_DIR}/target/dependency/include)
INCLUDE_DIRECTORIES (${MEC_SOURCE_DIR}/target/dependency/include/litesql)

LINK_DIRECTORIES(${LITESQL_DIR}/lib/static)
LINK_DIRECTORIES(${SAFMQ_DIR}/lib)

add_subdirectory (services)
add_subdirectory (hooks)
add_subdirectory (tasks)
