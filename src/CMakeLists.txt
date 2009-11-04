#ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB)
add_subdirectory (org)

include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)

include_directories (${WT_DIR}/include)
include_directories (${WT_DIR})
include_directories (${BOOST_INCLUDE_DIRS})

include_directories (${MYSQL_INCLUDE_DIR})
include_directories (${MYSQL_INCLUDE_DIR}/mysql)
include_directories (${FFMPEG_INCLUDE_DIR})
#MESSAGE("**MYSQL_INCLUDE_DIR: " ${MYSQL_INCLUDE_DIR})

include_directories (${BREAKPAD_INCLUDE_DIR})


LINK_DIRECTORIES(${BOOST_DIR}/lib)
LINK_DIRECTORIES(${FFMPEG_DIR}/lib)
LINK_DIRECTORIES(${BREAKPAD_LIB_DIR})


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

##LINK_DIRECTORIES(${MYSQL_DIR}/lib/debug)
#LINK_DIRECTORIES(${MYSQL_LIB_DIR})

#ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)
ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
#ADD_DEFINITIONS(-O0)
#ADD_DEFINITIONS(-g)
#set(CMAKE_FIND_LIBRARY_PREFIXES lib)
#set(CMAKE_FIND_LIBRARY_SUFFIXES .a)

#FIND_LIBRARY(avc avcodec)

#MESSAGE("** AVCODEC PATH: " ${AVCODEC_LIB})
#MESSAGE("** BOOST_SERIAL_LIB_MT: " ${BOOST_SERIAL_LIB_MT})


add_executable (mhive Hive.cpp)
SET_TARGET_PROPERTIES(mhive PROPERTIES INSTALL_RPATH lib)


#FIND_LIBRARY(COCOA_LIBRARY COCOA)
#MARK_AS_ADVANCED(COCOA_LIBRARY)
#SET(EXTRA_LIBS crypto ${COCOA_LIBRARY})
target_link_libraries (mhive 
  web 
  esb 
  sql 
  hive 
  lang 
  av 
  util 
  signal 
  io 
  net 
  ${BOOST_PO_LIB_MT}
)
IF(WIN32)
#  target_link_libraries (mhive  
#    wtd 
#    wtextd
#    wthttpd
#    winmm
#    "D:/google-breakpad/src/client/windows/Release/exception_handler.lib"
#  )
  
ELSEIF(APPLE)
  target_link_libraries (mhive 
    x264 
    ${MYSQL_LIBRARY} 
    ${BZ2_LIB}
#${COCOA_LIBRARY}
#    crypto
    z 
    ssl 
    wthttp 
    wtext 
    wt 
    pthread 
    z 
    mp3lame 
    theora 
    vorbis 
    vorbisenc 
    xvidcore 
    ogg
  )
ELSE(WIN32)
  target_link_libraries (mhive 
#    x264 
#    ssl
#    ${MYSQL_LIBRARY} 
#    ${BZ2_LIB} 
#    ssl
#    dl
#    crypt
#    crypto
#    pthread
#    z 
#    wthttp 
#    wtext 
#    wt 
#    mp3lame 
#    theora 
#    vorbis 
#    vorbisenc 
#    xvidcore 
#    ogg
  )
ENDIF(WIN32)

target_link_libraries (mhive
#  ${BOOST_SERIAL_LIB_MT}
#  ${BOOST_THREAD_LIB_MT}
  ${BOOST_SYSTEM_LIB_MT}
  ${SSL_LIB}
  ${CRYPTO_LIB}
#  ${BOOST_PO_LIB_MT}
#  ${BOOST_SIGNALS_LIB_MT}
#  ${BOOST_REGEX_LIB_MT}
#  ${BOOST_FS_LIB_MT}
#  ${BOOST_DT_LIB_MT}
)

SET(CURL_CMD curl)
SET(MHIVE_DBGSYM mhive)
SET(DUMPSYM_CMD "${BREAKPAD_DIR}/bin/dump_syms")
IF(WIN32)
  SET(CURL_CMD "${PROJECT_SOURCE_DIR}/win32/curl.exe")
  SET(DUMPSYM_CMD "${PROJECT_SOURCE_DIR}/win32/dump_syms.exe")
  SET(MHIVE_DBGSYM mhive.pdb)
ENDIF(WIN32)

add_custom_target(uploadsymbols
  COMMAND ${DUMPSYM_CMD} ${CMAKE_CURRENT_BINARY_DIR}/${MHIVE_DBGSYM} > ${CMAKE_CURRENT_BINARY_DIR}/mhive.sym
  COMMAND ${CURL_CMD} -F symupload=@${CMAKE_CURRENT_BINARY_DIR}/mhive.sym -F system=linux http://${SOCORRO_HOST}/symupload.php
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
INSTALL(FILES ${MYSQL_LIBRARY_DIR}/libmysqld.dll
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

