
add_subdirectory (org)
#add_subdirectory (erl)

include_directories (${MEC_SOURCE_DIR}/src)
##include_directories (${FFMPEG_DIR})
include_directories (${WT_DIR}/include)
include_directories (${WT_DIR})
#include_directories (${WT_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)

#include_directories (${MYSQL_INC_DIR})
#include_directories (${FFMPEG_DIR} ${FFMPEG_INC_DIR})
include_directories (${FFMPEG_DIR}/include)

LINK_DIRECTORIES(${WT_DIR}/lib)
#LINK_DIRECTORIES(${WT_DIR}/src)
#LINK_DIRECTORIES(${BOOST_LIB_DIR})
LINK_DIRECTORIES(${BOOST_DIR}/lib)
#LINK_DIRECTORIES(${WT_DIR}/src/http)
LINK_DIRECTORIES(${FFMPEG_DIR}/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libogg/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/x264/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/xvidcore/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/lame/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libtheora/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libvorbis/lib)


##LINK_DIRECTORIES(${FFMPEG_DIR}/libavcodec)
##LINK_DIRECTORIES(${FFMPEG_DIR}/libavutil)
##LINK_DIRECTORIES(${FFMPEG_DIR}/libswscale)
#LINK_DIRECTORIES(${X264_DIR}/lib)
LINK_DIRECTORIES(${MYSQL_DIR}/lib/mysql)
LINK_DIRECTORIES(${MYSQL_DIR}/Embedded/static/debug)
LINK_DIRECTORIES(${MYSQL_DIR}/lib/debug)
##LINK_DIRECTORIES(${MYSQL_DIR}/lib/debug)
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
FIND_LIBRARY(AVUTIL_LIB
    NAMES
      avutil
      libavutil
    PATHS
      ${FFMPEG_DIR}/lib
    NO_DEFAULT_PATH
  )
FIND_LIBRARY(SWSCALE_LIB
    NAMES
      swscale
      libswscale
    PATHS
      ${FFMPEG_DIR}/lib
    NO_DEFAULT_PATH
  )
#FIND_LIBRARY(avc avcodec)

#MESSAGE("** AVCODEC PATH: " ${AVCODEC_LIB})
MESSAGE("** BOOST_LIB PATH: " ${BOOST_LIB_DIR})


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


target_link_libraries (mhive  web esb sql  hive lang av util signal io net  avcodec avformat avutil swscale )
IF(WIN32)
target_link_libraries (mhive  ${MYSQL_LIB} wtd wtextd wthttpd winmm)
#target_link_libraries (mhive  ${MYSQL_LIB} wtd wtextd wthttpd libx264 libmp3lame libtheora libvorbis libvorbisenc xvidcore libogg)
ELSEIF(APPLE)
target_link_libraries (mhive x264 ${MYSQL_LIB} ${BZ2_LIB} z ssl wthttp wtext wt pthread z mp3lame theora vorbis vorbisenc xvidcore ogg)
ELSE(WIN32)
target_link_libraries (mhive x264 ${MYSQL_LIB} z ssl wthttp wtext wt pthread mp3lame theora vorbis vorbisenc xvidcore ogg)
ENDIF(WIN32)
MESSAGE("** BOOST_LIB PATH: " ${BOOST_THREAD_LIB_MT})

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

INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/web/
  DESTINATION ./web
  USE_SOURCE_PERMISSIONS
  PATTERN ".svn" EXCLUDE)

INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/res/
  DESTINATION ./res
  USE_SOURCE_PERMISSIONS
  PATTERN ".svn" EXCLUDE)

INSTALL(FILES ${PROJECT_SOURCE_DIR}/sql/hive-0.0.3.sql
  DESTINATION ./sql
  )

IF(WIN32)
INSTALL(FILES ${MYSQL_DIR}/Embedded/DLL/debug/libmysqld.dll
  DESTINATION ./bin
  )
INSTALL(FILES ${MYSQL_DIR}/share/english/errmsg.sys
  DESTINATION ./res
  )
INSTALL(FILES 
	${FFMPEG_DIR}/bin/avcodec-52.dll
	${FFMPEG_DIR}/bin/avdevice-52.dll
	${FFMPEG_DIR}/bin/avformat-52.dll
	${FFMPEG_DIR}/bin/avutil-50.dll
	${FFMPEG_DIR}/bin/swscale-0.dll
  DESTINATION ./bin
  )
ELSE(WIN32)
INSTALL(FILES ${MYSQL_DIR}/share/mysql/english/errmsg.sys
  DESTINATION ./res
  )
ENDIF(WIN32)

