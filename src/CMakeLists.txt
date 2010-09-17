#ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB)
ADD_DEFINITIONS(-D__STDC_LIMIT_MACROS)
#add_definitions(/DBOOST_AUTO_LINK_NOMANGLE)
add_definitions(-DBOOST_LIB_DIAGNOSTIC)
#add_definitions(/DBOOST_ALL_NO_LIB)
add_definitions(-DCURL_STATICLIB)
add_definitions(/DCURL_STATICLIB)

add_subdirectory (org)

include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MEC_SOURCE_DIR}/target/dependency/include/safmq)

include_directories (${WT_DIR}/include)
include_directories (${WT_DIR})
include_directories (${BOOST_INCLUDE_DIRS}/boost-1_36)

include_directories (${MYSQL_INCLUDE_DIR})
include_directories (${MYSQL_INCLUDE_DIR}/mysql)
include_directories (${FFMPEG_INCLUDE_DIR})
include_directories (${LOG4CPLUS_DIR}/include)
#MESSAGE("**MYSQL_INCLUDE_DIR: " ${MYSQL_INCLUDE_DIR})

include_directories (${BREAKPAD_INCLUDE_DIR})


LINK_DIRECTORIES(${BOOST_DIR}/lib)
LINK_DIRECTORIES(${FFMPEG_DIR}/lib)
LINK_DIRECTORIES(${BREAKPAD_LIB_DIR})
LINK_DIRECTORIES(${LOG4CPLUS_DIR}/lib)

#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libogg/lib)
#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/x264/lib)
#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/xvidcore/lib)
#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/lame/lib)
#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libtheora/lib)
#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libvorbis/lib)

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
add_executable (mhiveservice HiveService.cpp)
add_executable (mhiveclient MhiveClient.cpp)
add_executable (mhivemaster MHiveMaster.cpp)
#SET_TARGET_PROPERTIES(mhive PROPERTIES INSTALL_RPATH lib)


#FIND_LIBRARY(COCOA_LIBRARY COCOA)
#MARK_AS_ADVANCED(COCOA_LIBRARY)
#SET(EXTRA_LIBS crypto ${COCOA_LIBRARY})
target_link_libraries (mhiveservice io lang util)
 
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
  ${Boost_LIBRARIES}
#  log4cplus
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
#  target_link_libraries (mhive 
#    x264 
#    ${MYSQL_LIBRARY} 
#    ${BZ2_LIB}
#${COCOA_LIBRARY}
#    crypto
#    z 
#    ssl 
#    wthttp 
#    wtext 
#    wt 
#    pthread 
#    z 
#    mp3lame 
#    theora 
#    vorbis 
#    vorbisenc 
#    xvidcore 
#    ogg
#  )
ELSE(WIN32)
  target_link_libraries (mhive 
  rt
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

target_link_libraries (mhiveclient
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
)

target_link_libraries (mhivemaster
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
  mq
)
SET(CURL_CMD curl)
SET(ZIP_CMD gzip)
SET(MHIVE_DBGSYM mhive)
SET(DUMPSYM_CMD "${BREAKPAD_DIR}/bin/dump_syms")
IF(WIN32)
  SET(CURL_CMD "${PROJECT_SOURCE_DIR}/win32/curl.exe")
  SET(ZIP_CMD "${PROJECT_SOURCE_DIR}/win32/gzip.exe")
  SET(DUMPSYM_CMD "${PROJECT_SOURCE_DIR}/win32/dump_syms.exe")
  SET(MHIVE_DBGSYM mhive.pdb)
ENDIF(WIN32)

add_custom_target(uploadsymbols
  COMMAND ${DUMPSYM_CMD} ${CMAKE_CURRENT_BINARY_DIR}/${MHIVE_DBGSYM} > ${CMAKE_CURRENT_BINARY_DIR}/mhive.sym
  COMMAND ${ZIP_CMD} -f ${CMAKE_CURRENT_BINARY_DIR}/mhive.sym
  COMMAND ${CURL_CMD} -F symupload=@${CMAKE_CURRENT_BINARY_DIR}/mhive.sym.gz -F system=linux http://codergrid.de/symupload.php
)
MESSAGE("InstallBuilderExe $ENV{INSTALLBUILDER_EXE}")
add_custom_target(release
  COMMAND ${INSTALLBUILDER_EXE} build ${PROJECT_SOURCE_DIR}/iss/mhive.xml
)

INSTALL(TARGETS mhive
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
INSTALL(TARGETS mhiveservice
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})

INSTALL(TARGETS mhiveclient
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

#INSTALL(FILES 
#	${PROJECT_SOURCE_DIR}/sql/hive-0.0.3.sql
#	${PROJECT_SOURCE_DIR}/sql/hive-0.0.4.sql
#	${PROJECT_SOURCE_DIR}/sql/hive-0.0.5.sql
#	${PROJECT_SOURCE_DIR}/sql/hive-0.0.6.sql
# DESTINATION ./sql
#  )
INSTALL(FILES ${PROJECT_SOURCE_DIR}/sql/codec.txt
  DESTINATION ./sql
  )
INSTALL(FILES ${PROJECT_SOURCE_DIR}/sql/config.txt
  DESTINATION ./sql
  )
INSTALL(FILES ${PROJECT_SOURCE_DIR}/sql/profiles.txt
  DESTINATION ./sql
  )
IF(NOT WIN32)
  INSTALL(FILES ${PROJECT_SOURCE_DIR}/src/mectl
    DESTINATION ./bin
  )
ENDIF(NOT WIN32)

IF(WIN32)
  INSTALL(FILES ${MYSQL_LIBRARY_DIR}/libmysqld.dll
    DESTINATION ./bin
    )
  INSTALL(FILES ${MYSQL_DIR}/share/english/errmsg.sys
    DESTINATION ./res
    )
  INSTALL(FILES
      ${FFMPEG_DIR}/bin/avcodec-52.dll
#      ${FFMPEG_DIR}/bin/avcore-0.dll
      ${FFMPEG_DIR}/bin/avdevice-52.dll
      ${FFMPEG_DIR}/bin/avformat-52.dll
      ${FFMPEG_DIR}/bin/avutil-50.dll
      ${FFMPEG_DIR}/bin/swscale-0.dll
      ${CURL_DIR}/lib/libcurl-4.dll
    DESTINATION ./bin
    )
ELSEIF(APPLE)
  INSTALL(FILES ${MYSQL_DIR}/share/english/errmsg.sys
    DESTINATION ./res
    )
  INSTALL(FILES
      ${FFMPEG_DIR}/lib/libavcodec.52.dylib
      ${FFMPEG_DIR}/lib/libavdevice.52.dylib
      ${FFMPEG_DIR}/lib/libavformat.52.dylib
      ${FFMPEG_DIR}/lib/libavutil.50.dylib
      ${FFMPEG_DIR}/lib/libswscale.0.dylib
      ${CURL_DIR}/lib/libcurl.4.dylib
    DESTINATION ./lib
  )
ELSE(APPLE)
  INSTALL(FILES ${MYSQL_DIR}/share/english/errmsg.sys
    DESTINATION ./res
    )
  INSTALL(FILES
      ${FFMPEG_DIR}/lib/libavcodec.so.52
      ${FFMPEG_DIR}/lib/libavdevice.so.52
      ${FFMPEG_DIR}/lib/libavformat.so.52
      ${FFMPEG_DIR}/lib/libavutil.so.50
#      ${FFMPEG_DIR}/lib/libavcore.so.0
      ${FFMPEG_DIR}/lib/libswscale.so.0
      ${CURL_DIR}/lib/libcurl.so.4
    DESTINATION ./lib
    )

ENDIF(WIN32)

