
#add_subdirectory (org)

include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${FFMPEG_DIR}/include)
#include_directories (${WT_DIR}/src)
#include_directories (${WT_DIR})
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_INC_DIR})
LINK_DIRECTORIES(${WT_DIR}/lib)
LINK_DIRECTORIES(${BOOST_LIB_DIR})
LINK_DIRECTORIES(${BOOST_DIR}/lib)
LINK_DIRECTORIES(${WT_DIR}/lib)
LINK_DIRECTORIES(${WT_DIR}/src)
LINK_DIRECTORIES(${WT_DIR}/src/http)
LINK_DIRECTORIES(${FFMPEG_DIR}/lib)
LINK_DIRECTORIES(${X264_DIR}/lib)
LINK_DIRECTORIES(${MYSQL_DIR}/lib/mysql)
LINK_DIRECTORIES(${MYSQL_LIB_DIR})
LINK_DIRECTORIES(${PERFTOOLS_LIB})

ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)
IF(USE_EMBEDDED_MYSQL)
  IF(WIN32)
    SET(MYSQL_LIB libmysqld)
#    SET(MYSQL_LIB mysqlserver)
  ELSE(WIN32)
    SET(MYSQL_LIB mysqld)
  ENDIF(WIN32)
ELSE(USE_EMBEDDED_MYSQL)
  IF(WIN32)
    SET(MYSQL_LIB libmysql)
  ELSE(WIN32)
    SET(MYSQL_LIB mysqlclient)
  ENDIF(WIN32)
ENDIF(USE_EMBEDDED_MYSQL)


#add_executable (test_tcpsocket test_tcpsocket.cpp)
#target_link_libraries (test_tcpsocket lang esb sql hive av signal web io net util avcodec avformat avutil swscale wt wtext wthttp
#${BOOST_SERIAL_LIB_MT} ${BOOST_THREAD_LIB_MT} ${BOOST_SYSTEM_LIB_MT}  x264 mysqlclient)

#hive_LDADD= -L./org/esb -llang -lesb -lsql -lhive -lav -lutil -lsignal -lweb -lio -lnet -lwt -lwtext -lwthttp -lboost_serialization
add_executable (testav test_av.cpp)

target_link_libraries (testav web sql esb hive lang av util signal io net avformat avcodec avutil swscale )
IF(WIN32)
target_link_libraries (testav z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 Winmm a52 x264 ${MYSQL_LIB} wtd wtextd wthttpd)
#target_link_libraries (testav Winmm ${MYSQL_LIB} wtd wtextd wthttpd )
ELSE(WIN32)
target_link_libraries (testav x264 ${MYSQL_LIB} crypt ssl wthttp wtext wt pthread)
ENDIF(WIN32)


IF(NOT WIN32)
add_executable (test_sql test_sql.cpp)
add_executable (test_mysql test_mysql.cpp)
add_executable (info info.cpp)

target_link_libraries (test_sql sql esb util lang mysqlclient crypt ssl pthread ${BOOST_DT_LIB_MT})
target_link_libraries (info mysqlclient pthread esb web sql hive lang av util signal io net avformat avcodec avutil swscale x264 ${BOOST_DT_LIB_MT} ${BOOST_SYSTEM_LIB_MT} wthttp wtext wt)
target_link_libraries (test_mysql mysqlclient  ) 
ENDIF(NOT WIN32)

#target_link_libraries (testav esb lang av util io avformat avcodec avutil swscale x264 z pthread)
target_link_libraries (testav 
  ${BOOST_SERIAL_LIB_MT} 
  ${BOOST_THREAD_LIB_MT} 
  ${BOOST_SYSTEM_LIB_MT} 
  ${BOOST_PO_LIB_MT} 
  ${BOOST_SIGNALS_LIB_MT} 
  ${BOOST_REGEX_LIB_MT} 
  ${BOOST_FS_LIB_MT}
  ${BOOST_DT_LIB_MT}
)



