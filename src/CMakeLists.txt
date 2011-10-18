SET(LIB_INSTALL_DIR lib)
SET(PLUGIN_INSTALL_DIR plugin)
SET(BIN_INSTALL_DIR bin)
SET(WEB_INSTALL_DIR web)
SET(RES_INSTALL_DIR res)
SET(PRESETS_INSTALL_DIR presets)


IF(WIN32)
    SET(LIB_INSTALL_DIR bin)
#    SET(BIN_INSTALL_DIR bin)
#    SET(WEB_INSTALL_DIR web)
#    SET(RES_INSTALL_DIR res)
#    SET(PRESETS_INSTALL_DIR presets)
ELSEIF(APPLE)
    #SET(LIB_INSTALL_DIR MediaEncodingCluster.app/Contents/Resources/lib)
    #SET(BIN_INSTALL_DIR MediaEncodingCluster.app/Contents/Resources/bin)
    #SET(WEB_INSTALL_DIR MediaEncodingCluster.app/Contents/Resources/web)
    #SET(RES_INSTALL_DIR MediaEncodingCluster.app/Contents/Resources/res)
    #SET(PRESETS_INSTALL_DIR MediaEncodingCluster.app/Contents/Resources/presets)
ELSE(APPLE)
    #SET(LIB_INSTALL_DIR MediaEncodingCluster.app/Contents/Frameworks)
    #SET(BIN_INSTALL_DIR MediaEncodingCluster.app/Contents/MacOS)
    #SET(WEB_INSTALL_DIR MediaEncodingCluster.app/Contents/Resources/web)
    #SET(RES_INSTALL_DIR MediaEncodingCluster.app/Contents/Resources/res)
    #SET(PRESETS_INSTALL_DIR MediaEncodingCluster.app/Contents/Resources/presets)
ENDIF(WIN32)


#ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB)
ADD_DEFINITIONS(-D__STDC_LIMIT_MACROS)
#add_definitions(/DBOOST_AUTO_LINK_NOMANGLE)
#add_definitions(-DBOOST_LIB_DIAGNOSTIC)
#add_definitions(/DBOOST_ALL_NO_LIB)
add_definitions(-DCURL_STATICLIB)
add_definitions(/DCURL_STATICLIB)
#add_definitions(-m32)
#add_definitions(-arch i386)
#SET (CMAKE_EXE_LINKER_FLAGS "-arch i386")


#add_subdirectory (go)
add_subdirectory (org)
add_subdirectory (plugins)


SET(BUILD_MHIVESERVICE Off)
SET(BUILD_MHIVECLIENTAUDIO Off)
SET(BUILD_MHIVECLIENTVIDEO Off)
SET(BUILD_MHIVEQUEUE Off)
SET(BUILD_MHIVEWRITER Off)
SET(BUILD_MHIVEREADER Off)
SET(BUILD_MHIVECLIENT Off)
SET(BUILD_MHIVECORE Off)
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
#include_directories (${MEC_SOURCE_DIR}/target/dependency/include/safmq)
include_directories (${MEC_SOURCE_DIR}/target/dependency/include/litesql)

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
#add_executable (mhiveconfig MHiveConfig.cpp)
#target_link_libraries (mhiveconfig
#    api
#    )

#SET_TARGET_PROPERTIES (mhive
#  PROPERTIES BUILD_WITH_INSTALL_RPATH 1
#               INSTALL_NAME_DIR "@executable_path/../lib"
#	         )
		 
IF(BUILD_MHIVESERVICE)
    add_executable (mhiveservice HiveService.cpp)
    
    target_link_libraries (mhiveservice 
  io 
  lang 
  api
  util 
  hive 
  grid 
)
    INSTALL(TARGETS mhiveservice
        RUNTIME DESTINATION ${BIN_INSTALL_DIR}
        #BUNDLE DESTINATION ${BIN_INSTALL_DIR}
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR})

ENDIF(BUILD_MHIVESERVICE)

IF(BUILD_MHIVECLIENT)
    add_executable (mhiveclient MhiveClient.cpp)
    target_link_libraries (mhiveclient
      hive
      lang
      av
      util
      signal
      io
      net
    )
    INSTALL(TARGETS mhiveclient
        RUNTIME DESTINATION ${BIN_INSTALL_DIR}
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
ENDIF(BUILD_MHIVECLIENT)

IF(BUILD_MHIVECLIENTAUDIO)
    add_executable (mhiveca HiveClientAudio.cpp)
    target_link_libraries (mhiveca
      hive
      lang
      av
      util
      signal
      io
      net
    )
    INSTALL(TARGETS mhiveca
        RUNTIME DESTINATION ${BIN_INSTALL_DIR}
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
ENDIF(BUILD_MHIVECLIENTAUDIO)
IF(BUILD_MHIVECLIENTVIDEO)
    add_executable (mhivecv HiveClientVideo.cpp)
    target_link_libraries (mhivecv
      hive
      lang
      av
      util
      signal
      io
      net
    )
    INSTALL(TARGETS mhivecv
        RUNTIME DESTINATION ${BIN_INSTALL_DIR}
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
ENDIF(BUILD_MHIVECLIENTVIDEO)

IF(BUILD_MHIVEMASTER)
    add_executable (mhivemaster MHiveMaster.cpp)
    target_link_libraries (mhivemaster
      lang
      util
      io
      hive
    )
    INSTALL(TARGETS mhivemaster
        RUNTIME DESTINATION ${BIN_INSTALL_DIR}
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
ENDIF(BUILD_MHIVEMASTER)

IF(BUILD_MHIVEGRIDNODE)
    add_executable (mhivegridnode MHiveGridNode.cpp)
    target_link_libraries (mhivegridnode
      grid
    )
ENDIF(BUILD_MHIVEGRIDNODE)

IF(BUILD_MHIVEQUEUE)
    add_executable (mhivequeue MHiveQueue.cpp)
    target_link_libraries (mhivequeue)
    INSTALL(TARGETS mhivequeue
        RUNTIME DESTINATION ${BIN_INSTALL_DIR}
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
ENDIF(BUILD_MHIVEQUEUE)

IF(BUILD_MHIVEREADER)
    add_executable (mhivereader MHiveReader.cpp)
    target_link_libraries (mhivereader lang esb util)
    INSTALL(TARGETS mhivereader
        RUNTIME DESTINATION ${BIN_INSTALL_DIR}
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
ENDIF(BUILD_MHIVEREADER)

IF(BUILD_MHIVEWRITER)
    add_executable (mhivewriter MHiveWriter.cpp)
    target_link_libraries (mhivewriter lang)
    INSTALL(TARGETS mhivewriter
        RUNTIME DESTINATION ${BIN_INSTALL_DIR}
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
ENDIF(BUILD_MHIVEWRITER)

IF(BUILD_MHIVECORE)
    add_executable (mhivecore MHiveCore.cpp)
    target_link_libraries (mhivecore lang core hive )
    INSTALL(TARGETS mhivecore
        RUNTIME DESTINATION ${BIN_INSTALL_DIR}
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
ENDIF(BUILD_MHIVECORE)

#SET_TARGET_PROPERTIES(mhive PROPERTIES INSTALL_RPATH lib)


#FIND_LIBRARY(COCOA_LIBRARY COCOA)
#MARK_AS_ADVANCED(COCOA_LIBRARY)
#SET(EXTRA_LIBS crypto ${COCOA_LIBRARY})
 
target_link_libraries (mhive 
  lang
  hive 
  av 
  util 
  signal 
  io 
  net
  rpc
  core

  ${Boost_LIBRARIES}
#  log4cplus
#  PocoNet
)
IF(WIN32)
  IF(CMAKE_BUILD_TYPE MATCHES "debug")
    TARGET_LINK_LIBRARIES(mhive log4cplusd)
    ELSE()
    TARGET_LINK_LIBRARIES(mhive log4cplus)
  ENDIF()

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
#  rt
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


#SET(CURL_CMD curl)
#SET(ZIP_CMD gzip)
#SET(MHIVE_DBGSYM mhive)
#SET(DUMPSYM_CMD "${BREAKPAD_DIR}/bin/dump_syms")
#IF(WIN32)
#  SET(CURL_CMD "${PROJECT_SOURCE_DIR}/win32/curl.exe")
#  SET(ZIP_CMD "${PROJECT_SOURCE_DIR}/win32/gzip.exe")
#  SET(DUMPSYM_CMD "${PROJECT_SOURCE_DIR}/win32/dump_syms.exe")
#  SET(MHIVE_DBGSYM mhive.pdb)
#ENDIF(WIN32)


add_custom_target(uploadsymbols
  COMMAND ${DUMPSYM_CMD} ${CMAKE_CURRENT_BINARY_DIR}/${MHIVE_DBGSYM} > ${CMAKE_CURRENT_BINARY_DIR}/mhive.sym
  COMMAND ${ZIP_CMD} -f ${CMAKE_CURRENT_BINARY_DIR}/mhive.sym
  COMMAND ${CURL_CMD} -F symupload=@${CMAKE_CURRENT_BINARY_DIR}/mhive.sym.gz -F system=linux http://codergrid.de/symupload.php
)
#add_custom_target(upload
#  COMMAND ${CURL_CMD} -T ${PROJECT_SOURCE_DIR}/build/MediaEncodingCluster-${CPACK_PACKAGE_VERSION}-Darwin.dmg -u${userid}:${pass} ftp://codergrid.de
#)
MESSAGE("InstallBuilderExe $ENV{INSTALLBUILDER_EXE}")
add_custom_target(release
  COMMAND ${INSTALLBUILDER_EXE} build ${PROJECT_SOURCE_DIR}/iss/mhive.xml
)





INSTALL(TARGETS mhive
    RUNTIME DESTINATION ${BIN_INSTALL_DIR}
#    COMPONENT mhive
)

#INSTALL(TARGETS mhiveconfig
#    RUNTIME DESTINATION ${BIN_INSTALL_DIR}
#    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
#    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})

INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/web/
  DESTINATION ${WEB_INSTALL_DIR}
  USE_SOURCE_PERMISSIONS
  PATTERN ".svn" EXCLUDE
  PATTERN "www" EXCLUDE
)
#INSTALL(CODE "EXECUTE_PROCESS(COMMAND cp ${PROJECT_SOURCE_DIR}/LICENSE ${CMAKE_INSTALL_PREFIX}/LICENSE)")
INSTALL(FILES ${PROJECT_SOURCE_DIR}/LICENSE
    DESTINATION .
    PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE
)

#INSTALL(CODE "EXECUTE_PROCESS(COMMAND ln -s ${PROJECT_SOURCE_DIR}/web ${CMAKE_INSTALL_PREFIX}/web)")

INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/res/
  DESTINATION ${RES_INSTALL_DIR}
  USE_SOURCE_PERMISSIONS
  PATTERN ".svn" EXCLUDE
  )

#SET(MHIVE_BASE ${CMAKE_INSTALL_PREFIX})
#CONFIGURE_FILE(${CMAKE_INSTALL_PREFIX}/res/logging.properties ${CMAKE_INSTALL_PREFIX}/res/logging.properties)
#CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/mectl ${CMAKE_INSTALL_PREFIX}/bin/mectl.out)

INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/presets/
  DESTINATION ${PRESETS_INSTALL_DIR}
#  USE_SOURCE_PERMISSIONS
  PATTERN ".svn" EXCLUDE)

IF(NOT WIN32)
  INSTALL(FILES ${PROJECT_SOURCE_DIR}/src/mectl
    DESTINATION ${BIN_INSTALL_DIR}
    PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE
  )
ENDIF(NOT WIN32)

IF(WIN32)
  INSTALL(FILES
      ${FFMPEG_DIR}/lib/avcodec-53.dll
#      ${FFMPEG_DIR}/lib/avcore.dll
      ${FFMPEG_DIR}/lib/avdevice-53.dll
      ${FFMPEG_DIR}/lib/avformat-53.dll
      ${FFMPEG_DIR}/lib/avutil-51.dll
      ${FFMPEG_DIR}/lib/swscale-2.dll
    DESTINATION ${LIB_INSTALL_DIR}
    )
ELSEIF(APPLE)
  INSTALL(FILES
      ${FFMPEG_DIR}/lib/libavcodec.dylib
#      ${FFMPEG_DIR}/lib/libavcore.dylib
      ${FFMPEG_DIR}/lib/libavdevice.dylib
      ${FFMPEG_DIR}/lib/libavformat.dylib
      ${FFMPEG_DIR}/lib/libavutil.dylib
      ${FFMPEG_DIR}/lib/libswscale.dylib
    DESTINATION ${LIB_INSTALL_DIR}
#    COMPONENT libraries
  )
  SET_SOURCE_FILES_PROPERTIES(
  ${PROJECT_SOURCE_DIR}/src/mectl
  PROPERTIES
  MACOSX_PACKAGE_LOCATION MacOS
  
  )
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E create_symlink ${BIN_INSTALL_DIR}/mectl ${BIN_INSTALL_DIR}/mectl123)")
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND mkdir ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/../../MacOS)")
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND ln -s ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mectl ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/../../MacOS/mectl)")
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND cp ${PROJECT_SOURCE_DIR}/Info.plist ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/../..)")
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND cp ${PROJECT_SOURCE_DIR}/Info.plist ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/../../Resources/res/tmp.list)")
#  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/debug/label/mac/trunk/source/build/ffmpeg/lib/libavcore.dylib @executable_path/../lib/libavcore.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libavcodec.dylib @executable_path/../lib/libavcodec.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libavformat.dylib @executable_path/../lib/libavformat.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libavutil.dylib @executable_path/../lib/libavutil.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libswscale.dylib @executable_path/../lib/libswscale.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libavcodec.dylib @executable_path/../lib/libavcodec.dylib ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libavformat.dylib)")
#  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libavcore.dylib @executable_path/../lib/libavcore.dylib ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libavformat.dylib)")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libavutil.dylib @executable_path/../lib/libavutil.dylib ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libavformat.dylib)")
#  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libavcore.dylib @executable_path/../lib/libavcore.dylib ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libavcodec.dylib)")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libavutil.dylib @executable_path/../lib/libavutil.dylib ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libavcodec.dylib)")
#  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libavutil.dylib @executable_path/../lib/libavutil.dylib ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libavcore.dylib)")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libavutil.dylib @executable_path/../lib/libavutil.dylib ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libswscale.dylib)")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change libmhive-util.dylib @executable_path/../lib/libmhive-util.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change libmhive-signal.dylib @executable_path/../lib/libmhive-signal.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change libmhive-io.dylib @executable_path/../lib/libmhive-io.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change libmhive-util.dylib @executable_path/../lib/libmhive-util.dylib ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libmhive-signal.dylib)")

ELSE(APPLE)
  INSTALL(FILES
      ${FFMPEG_DIR}/lib/libavcodec.so.53
      ${FFMPEG_DIR}/lib/libavdevice.so.53
      ${FFMPEG_DIR}/lib/libavformat.so.53
      ${FFMPEG_DIR}/lib/libavutil.so.51
#      ${FFMPEG_DIR}/lib/libavcore.so
      ${FFMPEG_DIR}/lib/libswscale.so.2
#      ${FFMPEG_DIR}/lib/libPocoNet.so.11
#      ${FFMPEG_DIR}/lib/libPocoFoundation.so.11
    DESTINATION ${LIB_INSTALL_DIR}
    )
    #INSTALL(CODE "CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/mectl ${CMAKE_INSTALL_PREFIX}/bin/mectl)")

ENDIF(WIN32)

