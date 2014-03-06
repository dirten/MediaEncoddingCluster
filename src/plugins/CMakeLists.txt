INCLUDE_DIRECTORIES (${MEC_SOURCE_DIR}/src)
include_directories (${DEPS_INC_DIR})
include_directories (${DEPS_INC_DIR}/litesql)

LINK_DIRECTORIES(${DEPS_LIB_DIR})
LINK_DIRECTORIES(${DEPS_LIB_DIR}/static)

add_subdirectory (services)
add_subdirectory (hooks)
add_subdirectory (tasks)
add_subdirectory (MHiveStorage)
