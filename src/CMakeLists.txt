
add_subdirectory (org)

include_directories (${MEC_SOURCE_DIR}/src)
#include_directories (${FFMPEG_DIR})
include_directories (${WT_DIR}/include)
include_directories (${WT_DIR})
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${MYSQL_INC_DIR})
include_directories (${FFMPEG_DIR} ${FFMPEG_INC_DIR})

LINK_DIRECTORIES(${WT_DIR}/lib)
LINK_DIRECTORIES(${BOOST_LIB_DIR})
LINK_DIRECTORIES(${BOOST_DIR}/lib)
LINK_DIRECTORIES(${WT_DIR}/src/http)
LINK_DIRECTORIES(${FFMPEG_DIR}/lib)
#LINK_DIRECTORIES(${FFMPEG_DIR}/libavcodec)
#LINK_DIRECTORIES(${FFMPEG_DIR}/libavutil)
#LINK_DIRECTORIES(${FFMPEG_DIR}/libswscale)
LINK_DIRECTORIES(${X264_DIR}/lib)
LINK_DIRECTORIES(${MYSQL_LIB_DIR})

ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)
#set(CMAKE_FIND_LIBRARY_PREFIXES lib)
#set(CMAKE_FIND_LIBRARY_SUFFIXES .a)

FIND_LIBRARY(AVCODEC_LIB
    NAMES
      avcodec
      libavcodec
    PATHS
      ${FFMPEG_DIR}/lib
    NO_DEFAULT_PATH
  )
FIND_LIBRARY(AVFORMAT_LIB
    NAMES
      avformat
      libavformat
    PATHS
      ${FFMPEG_DIR}/lib
    NO_DEFAULT_PATH
  )
#FIND_LIBRARY(avc avcodec)
MESSAGE("** AVCODEC PATH: " ${AVCODEC_LIB})
MESSAGE("** BOOST_LIB PATH: " ${BOOST_LIB_DIR})


add_executable (mhive Hive.cpp)

target_link_libraries (mhive lang esb sql web hive av util signal io net avcodec avformat avutil swscale wtd wtextd wthttpd
${BOOST_SERIAL_LIB_MT} ${BOOST_THREAD_LIB_MT} ${BOOST_SYSTEM_LIB_MT}  x264 )
IF(WIN32)
target_link_libraries (mhive z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 Winmm a52 libmysql)
ELSE(WIN32)
target_link_libraries (mhive mysqlclient)
ENDIF(WIN32)

#hive_LDADD= -L./org/esb -llang -lesb -lsql -lhive -lav -lutil -lsignal -lweb -lio -lnet -lwt -lwtext -lwthttp -lboost_serialization
