
add_subdirectory (org)

include_directories (${MEC_SOURCE_DIR}/src)
#include_directories (${FFMPEG_DIR})
include_directories (${WT_DIR}/src)
include_directories (${WT_DIR})
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${MYSQL_INC_DIR})
include_directories (${FFMPEG_DIR} ${FFMPEG_INC_DIR})

LINK_DIRECTORIES(${WT_DIR}/src)
LINK_DIRECTORIES(${BOOST_LIB_DIR})
LINK_DIRECTORIES(${WT_DIR}/src/http)
LINK_DIRECTORIES(${FFMPEG_DIR}/lib)
#LINK_DIRECTORIES(${FFMPEG_DIR}/libavcodec)
#LINK_DIRECTORIES(${FFMPEG_DIR}/libavutil)
#LINK_DIRECTORIES(${FFMPEG_DIR}/libswscale)
LINK_DIRECTORIES(${X264_DIR}/lib)
LINK_DIRECTORIES(${MYSQL_LIB_DIR})

ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)

add_executable (mhive Hive.cpp)
target_link_libraries (mhive lang esb sql web hive av util signal io net  avcodec avformat avutil swscale wt wtext wthttp
${BOOST_SERIAL_LIB_MT} ${BOOST_THREAD_LIB_MT} ${BOOST_SYSTEM_LIB_MT}  x264 mysqlclient)

#hive_LDADD= -L./org/esb -llang -lesb -lsql -lhive -lav -lutil -lsignal -lweb -lio -lnet -lwt -lwtext -lwthttp -lboost_serialization
