
add_subdirectory (org)

include_directories (${MEC_SOURCE_DIR}/src)
#include_directories (${FFMPEG_DIR})
include_directories (${WT_DIR}/include)
include_directories (${WT_DIR})
include_directories (${WT_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_INC_DIR})
include_directories (${FFMPEG_DIR} ${FFMPEG_INC_DIR})
include_directories (${FFMPEG_DIR}/include)

LINK_DIRECTORIES(${WT_DIR}/lib)
LINK_DIRECTORIES(${WT_DIR}/src)
LINK_DIRECTORIES(${BOOST_LIB_DIR})
LINK_DIRECTORIES(${BOOST_DIR}/lib)
LINK_DIRECTORIES(${WT_DIR}/src/http)
LINK_DIRECTORIES(${FFMPEG_DIR}/lib)
#LINK_DIRECTORIES(${FFMPEG_DIR}/libavcodec)
#LINK_DIRECTORIES(${FFMPEG_DIR}/libavutil)
#LINK_DIRECTORIES(${FFMPEG_DIR}/libswscale)
LINK_DIRECTORIES(${X264_DIR}/lib)
LINK_DIRECTORIES(${MYSQL_DIR}/lib/mysql)
LINK_DIRECTORIES(${MYSQL_DIR}/lib/debug)
LINK_DIRECTORIES(${MYSQL_LIB_DIR})

ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)
ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
#ADD_DEFINITIONS(-O0)
#ADD_DEFINITIONS(-g)
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

#MESSAGE("** AVCODEC PATH: " ${AVCODEC_LIB})
#MESSAGE("** BOOST_LIB PATH: " ${BOOST_LIB_DIR})


add_executable (mhive Hive.cpp)
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


target_link_libraries (mhive web esb sql  hive lang av util signal io net  avcodec avformat avutil swscale )
IF(WIN32)
target_link_libraries (mhive z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 Winmm a52 x264 ${MYSQL_LIB} wt wtext wthttp)
#target_link_libraries (mhive Winmm ${MYSQL_LIB} wt wtext wthttp )
ELSE(WIN32)
target_link_libraries (mhive x264 ${MYSQL_LIB} crypt ssl wthttp wtext wt pthread)
ENDIF(WIN32)

target_link_libraries (mhive 
  ${BOOST_SERIAL_LIB_MT} 
  ${BOOST_THREAD_LIB_MT} 
  ${BOOST_SYSTEM_LIB_MT} 
  ${BOOST_PO_LIB_MT} 
  ${BOOST_SIGNALS_LIB_MT} 
  ${BOOST_REGEX_LIB_MT} 
  ${BOOST_FS_LIB_MT}
  ${BOOST_DT_LIB_MT}
)



INSTALL(TARGETS mhive
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
