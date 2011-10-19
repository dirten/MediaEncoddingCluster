include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MEC_SOURCE_DIR})
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)


add_executable (DownloadTaskTest DownloadTaskTest.cpp)

target_link_libraries (DownloadTaskTest downloadtask)

get_target_property(DOWNLOADTASK_PLUGIN downloadtask LOCATION)

ADD_DEFINITIONS(-DDOWNLOADTASK_PLUGIN="${DOWNLOADTASK_PLUGIN}")


