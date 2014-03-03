SET( COREAPI_VERSION 0.1.0 )

SET(LIB_INSTALL_DIR ${MHIVE_VERSION}/lib)
SET(PLUGIN_INSTALL_DIR ${MHIVE_VERSION}/plugin)
SET(BIN_INSTALL_DIR ${MHIVE_VERSION}/bin)
SET(WEB_INSTALL_DIR ${MHIVE_VERSION}/web)
SET(RES_INSTALL_DIR ${MHIVE_VERSION}/res)
SET(PRESETS_INSTALL_DIR ${MHIVE_VERSION}/presets)


IF(WIN32)
    SET(LIB_INSTALL_DIR ${MHIVE_VERSION}/bin)
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


ADD_DEFINITIONS(-D__STDC_LIMIT_MACROS)
#ADD_DEFINITIONS("-Wl,--allow-multiple-definition")



#add_subdirectory (go)
ADD_DEFINITIONS(-DMHIVE_VERSION="${MHIVE_VERSION}")

add_subdirectory (org)
add_subdirectory (plugins)


include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${DEPS_INC_DIR})
include_directories (${DEPS_INC_DIR}/litesql)

LINK_DIRECTORIES(${DEPS_LIB_DIR})
LINK_DIRECTORIES(${LITESQL_DIR}/lib/static)
#LINK_DIRECTORIES(${LITESQL_DIR}/lib)


ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)

add_executable (mhive Main.cpp )

target_link_libraries (mhive 
  #${Boost_LIBRARIES}
  core
  av
  io
  #${POCO_LIB_FOUNDATION}
  #${POCO_LIB_FOUNDATION}
)
IF(WIN32)
  IF(CMAKE_BUILD_TYPE MATCHES "debug")
    #TARGET_LINK_LIBRARIES(mhive log4cplusd)
  ELSE()
    #TARGET_LINK_LIBRARIES(mhive log4cplus)
  ENDIF()
ELSEIF(APPLE)

ELSE(WIN32)

ENDIF(WIN32)

# SymbolFile Generation for google-breakpad and socorro server
add_custom_target(uploadsymbols
  COMMAND ${DUMPSYM_CMD} ${CMAKE_CURRENT_BINARY_DIR}/${MHIVE_DBGSYM} > ${CMAKE_CURRENT_BINARY_DIR}/mhive.sym
  COMMAND ${ZIP_CMD} -f ${CMAKE_CURRENT_BINARY_DIR}/mhive.sym
  COMMAND ${CURL_CMD} -F symupload=@${CMAKE_CURRENT_BINARY_DIR}/mhive.sym.gz -F system=linux http://codergrid.de/symupload.php
)

# installer creation
add_custom_target(release
  COMMAND ${INSTALLBUILDER_EXE} build ${PROJECT_SOURCE_DIR}/iss/mhive.xml
)

INSTALL(TARGETS mhive
    RUNTIME DESTINATION ${BIN_INSTALL_DIR}
#    COMPONENT mhive
)


INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/web/
  DESTINATION ${WEB_INSTALL_DIR}
  USE_SOURCE_PERMISSIONS
  PATTERN ".svn" EXCLUDE
  PATTERN "www" EXCLUDE
)

#INSTALL(FILES ${PROJECT_SOURCE_DIR}/LICENSE
#    DESTINATION .
#   PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE
#)

INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/res/
  DESTINATION ${RES_INSTALL_DIR}
  USE_SOURCE_PERMISSIONS
  PATTERN ".svn" EXCLUDE
  )

INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/presets/
  DESTINATION ${PRESETS_INSTALL_DIR}
  PATTERN ".svn" EXCLUDE)

IF(NOT WIN32)
  INSTALL(FILES ${PROJECT_SOURCE_DIR}/src/mectl
    DESTINATION ${BIN_INSTALL_DIR}
    PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE
  )
ENDIF(NOT WIN32)

IF(WIN32)
#  INSTALL(FILES
#      ${FFMPEG_DIR}/${FFMPEG_LIB_DIR}/avcodec-55.dll
#      ${FFMPEG_DIR}/${FFMPEG_LIB_DIR}/avcore.dll
#      ${FFMPEG_DIR}/${FFMPEG_LIB_DIR}/avdevice-55.dll
#      ${FFMPEG_DIR}/${FFMPEG_LIB_DIR}/avformat-55.dll
#      ${FFMPEG_DIR}/${FFMPEG_LIB_DIR}/avutil-52.dll
#      ${FFMPEG_DIR}/${FFMPEG_LIB_DIR}/swscale-2.dll
#    DESTINATION ${LIB_INSTALL_DIR}
#    )
ELSEIF(APPLE)
  INSTALL(DIRECTORY ${FFMPEG_DIR}/lib/
    DESTINATION ${LIB_INSTALL_DIR}
    FILES_MATCHING PATTERN "*.dylib*"
    PATTERN ".svn" EXCLUDE
  )

#  INSTALL(FILES
#      ${FFMPEG_DIR}/lib/libavcodec.dylib
#      ${FFMPEG_DIR}/lib/libavcore.dylib
#      ${FFMPEG_DIR}/lib/libavdevice.dylib
#      ${FFMPEG_DIR}/lib/libavformat.dylib
#      ${FFMPEG_DIR}/lib/libavutil.dylib
#      ${FFMPEG_DIR}/lib/libavfilter.dylib
#      ${FFMPEG_DIR}/lib/libswscale.dylib
#      ${FFMPEG_DIR}/lib/libswresample.dylib
#    DESTINATION ${LIB_INSTALL_DIR}
#    COMPONENT libraries
#  )
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

  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change ${FFMPEG_DIR}/lib/libavcodec.55.dylib libavcodec.55.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change ${FFMPEG_DIR}/lib/libavdevice.55.dylib libavdevice.55.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change ${FFMPEG_DIR}/lib/libavutil.52.dylib libavutil.52.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change ${FFMPEG_DIR}/lib/libavformat.55.dylib libavformat.55.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change ${FFMPEG_DIR}/lib/libswresample.0.dylib libswresample.0.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change ${FFMPEG_DIR}/lib/libswscale.2.dylib libswscale.2.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change ${FFMPEG_DIR}/lib/libavfilter.3.dylib libavfilter.3.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")

  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change ${FFMPEG_DIR}/lib/libavcodec.55.dylib libavcodec.55.dylib ${FFMPEG_DIR}/lib/libavcodec.55.dylib)")
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change ${FFMPEG_DIR}/lib/libavutil.52.dylib libavutil.52.dylib ${FFMPEG_DIR}/lib/libavcodec.55.dylib)")
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libavutil.dylib @executable_path/../lib/libavutil.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libswscale.dylib @executable_path/../lib/libswscale.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libavcodec.dylib @executable_path/../lib/libavcodec.dylib ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libavformat.dylib)")
#  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libavcore.dylib @executable_path/../lib/libavcore.dylib ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libavformat.dylib)")
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libavutil.dylib @executable_path/../lib/libavutil.dylib ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libavformat.dylib)")
#  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libavcore.dylib @executable_path/../lib/libavcore.dylib ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libavcodec.dylib)")
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libavutil.dylib @executable_path/../lib/libavutil.dylib ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libavcodec.dylib)")
#  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libavutil.dylib @executable_path/../lib/libavutil.dylib ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libavcore.dylib)")
  #INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change /private/tmp/workspace/ffmpeg_static/build_type/release/label/mac/trunk/source/build/ffmpeg/lib/libavutil.dylib @executable_path/../lib/libavutil.dylib ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libswscale.dylib)")
#  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change libmhive-util.dylib @executable_path/../lib/libmhive-util.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
#  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change libmhive-signal.dylib @executable_path/../lib/libmhive-signal.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
#  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change libmhive-io.dylib @executable_path/../lib/libmhive-io.dylib ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/mhive)")
#  INSTALL(CODE "EXECUTE_PROCESS(COMMAND install_name_tool -change libmhive-util.dylib @executable_path/../lib/libmhive-util.dylib ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libmhive-signal.dylib)")

ELSE(APPLE)
INSTALL(DIRECTORY ${FFMPEG_DIR}/lib/
  DESTINATION ${LIB_INSTALL_DIR}
  FILES_MATCHING PATTERN "*.so*"
  PATTERN ".svn" EXCLUDE
)

#  INSTALL(FILES
#      ${FFMPEG_DIR}/lib/libavcodec.so.55
#      ${FFMPEG_DIR}/lib/libavdevice.so.55
#      ${FFMPEG_DIR}/lib/libavformat.so.55
#      ${FFMPEG_DIR}/lib/libavutil.so.52
#      ${FFMPEG_DIR}/lib/libavfilter.so.3
#      ${FFMPEG_DIR}/lib/libavcore.so
#      ${FFMPEG_DIR}/lib/libswscale.so.2
#      ${FFMPEG_DIR}/lib/libswresample.so.0
#      ${FFMPEG_DIR}/lib/libPocoNet.so.11
#      ${FFMPEG_DIR}/lib/libPocoFoundation.so.11
#    DESTINATION ${LIB_INSTALL_DIR}
#    )
    #INSTALL(CODE "CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/mectl ${CMAKE_INSTALL_PREFIX}/bin/mectl)")

ENDIF(WIN32)

