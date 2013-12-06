INCLUDE_DIRECTORIES (${FFMPEG_INCLUDE_DIR})
INCLUDE_DIRECTORIES (${MEC_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES (${MEC_SOURCE_DIR}/target/dependency/include)
INCLUDE_DIRECTORIES (${MEC_SOURCE_DIR}/target/dependency/include/litesql)
include_directories (${SAFMQ_DIR}/include/lib)
include_directories (${SAFMQ_DIR}/include/shared)

LINK_DIRECTORIES(${LITESQL_DIR}/lib/static)
LINK_DIRECTORIES(${DEPS_LIB_DIR})

add_subdirectory (services)
add_subdirectory (hooks)
add_subdirectory (tasks)
