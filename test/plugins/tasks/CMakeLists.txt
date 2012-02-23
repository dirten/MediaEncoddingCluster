include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MEC_SOURCE_DIR})
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)


add_executable (DownloadTaskTest DownloadTaskTest.cpp)
add_executable (HTTPPullSourceTest HTTPPullSourceTest.cpp)
add_executable (EncodingTaskTest EncodingTaskTest.cpp)
add_executable (ExportTaskTest ExportTaskTest.cpp)
add_executable (StreamPacketizerTest StreamPacketizerTest.cpp)

target_link_libraries (HTTPPullSourceTest core)
target_link_libraries (DownloadTaskTest downloadtask)
target_link_libraries (EncodingTaskTest encodingtask exporttask)
target_link_libraries (ExportTaskTest encodingtask exporttask)
target_link_libraries (StreamPacketizerTest encodingtask)

get_target_property(HTTPPULLSOURCE_PLUGIN httppullsource LOCATION)
ADD_DEFINITIONS(-DHTTPPULLSOURCE_PLUGIN="${HTTPPULLSOURCE_PLUGIN}")

get_target_property(DOWNLOADTASK_PLUGIN downloadtask LOCATION)
ADD_DEFINITIONS(-DDOWNLOADTASK_PLUGIN="${DOWNLOADTASK_PLUGIN}")

get_target_property(ENCODINGTASK_PLUGIN encodingtask LOCATION)
ADD_DEFINITIONS(-DENCODINGTASK_PLUGIN="${ENCODINGTASK_PLUGIN}")

get_target_property(EXPORTTASK_PLUGIN exporttask LOCATION)
ADD_DEFINITIONS(-DEXPORTTASK_PLUGIN="${EXPORTTASK_PLUGIN}")


