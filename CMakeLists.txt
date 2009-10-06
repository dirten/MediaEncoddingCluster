############################################################
#
# Main Cmake file
#
############################################################
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project (MEC)

INCLUDE(CTest)

ENABLE_TESTING()


INCLUDE(cmake/HiveFindBoost.txt)
INCLUDE(cmake/HiveFindMysql.txt)
INCLUDE(cmake/HiveFindFFmpeg.txt)
INCLUDE(cmake/HiveFindBreakpad.txt)

############################################################
#
# Setup the version for MHive
#
############################################################
SET(MAJOR_VERSION 0)
SET(MINOR_VERSION 0)
SET(PATCH_VERSION 4)
SET(MINOR_PATCH_VERSION 5)
SET(MHIVE_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}.${MINOR_PATCH_VERSION})

############################################################
#
# Build Type Options
#
############################################################

IF(CMAKE_BUILD_TYPE MATCHES "Debug")
  ADD_DEFINITIONS(-DDEBUG)
ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")

############################################################
#
# Settings for the CPAck Packaging
# on windows the packages will be build with the inno setup
#
############################################################
SET(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}.${MINOR_PATCH_VERSION}")
SET(CPACK_PACKAGE_NAME "MediaEncodingCluster")
SET(CPACK_PACKAGE_VENDOR "EsbLab")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY MediaEncodingCluster-${MHIVE_VERSION})
SET(CPACK_STRIP_FILES mhive)
IF(WIN32)
  SET(CPACK_GENERATOR "NSIS" )
#  SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS  
#      "File \\\"c:\\\\vcredist_x86.exe\\\"
#      ExecWait '\\\"$INSTDIR\\\\vcredist_x86.exe\\\" /q:a'
#      CreateShortCut \\\"$SMPROGRAMS\\\\\$STARTMENU_FOLDER\\\\Start Mhive.lnk" "$INSTDIR\\\\bin\\\\mhive.exe -s\\\"
#  ")
ELSEIF(APPLE)
  SET(CPACK_GENERATOR "TGZ;PackageMaker;TZ;TBZ2" )
ELSE(WIN32)
  SET(CPACK_GENERATOR "TGZ;TBZ2" )
ENDIF(WIN32)

############################################################
#
# Find the libbz2, it is needed for ffmpeg when visible
#
############################################################
FIND_LIBRARY(LIBBZ2
  NAMES
    bz2
  PATHS
    /usr/lib
  )
IF(LIBBZ2)
  SET(BZ2_LIB ${LIBBZ2})
ENDIF(LIBBZ2)
############################################################
#
# Setup build Defines
#
############################################################
IF(UNIX)
  ADD_DEFINITIONS(-D__UNIX__)
  IF(APPLE)
    ADD_DEFINITIONS(-D__APPLE__)
  ELSE(APPLE)
    ADD_DEFINITIONS(-D__LINUX__)
  ENDIF(APPLE)
ELSEIF(WIN32)
  ADD_DEFINITIONS(-D__WIN32__)
ENDIF(UNIX)



LINK_DIRECTORIES(${MYSQL_LIBRARY_PATH})
LINK_DIRECTORIES(${WT_DIR}/lib)

SET(BUILD_SHARED_LIBS OFF)
SET(BUILD_EMBEDDED_LIBS OFF CACHE STRING "Use Embedded Dependency Libraries")
SET(USE_EMBEDDED_MYSQL ON CACHE STRING "Use Embedded Mysql")



# Build Embedded libraries in deps directory
IF(BUILD_EMBEDDED_LIBS)
  FILE(APPEND ${PROJECT_SOURCE_DIR}/deps/stage/boost.build "")
  FILE(APPEND ${PROJECT_SOURCE_DIR}/deps/stage/wt.build "")
  FILE(APPEND ${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg.download "")
  FILE(APPEND ${PROJECT_SOURCE_DIR}/deps/stage/x264.download "")
  FILE(APPEND ${PROJECT_SOURCE_DIR}/deps/stage/mp3lame.download "")




#  MESSAGE("** Downloading erlang runtime")
#  FILE(DOWNLOAD http://erlang.org/download/otp_src_R13B.tar.gz bla/otp_src_R13B.tar.gz)

  MESSAGE("** Building Embedded Libraries, that will take several time")

  FILE(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/stage)
  FILE(APPEND ${PROJECT_SOURCE_DIR}/deps/stage/boost.build "")
  FILE(APPEND ${PROJECT_SOURCE_DIR}/deps/stage/wt.build "")
  FILE(APPEND ${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg.build "")
  FILE(APPEND ${PROJECT_SOURCE_DIR}/deps/stage/x264.build "")

# setup to build embedded libraries
  SET(BUILD_EMBEDDED_BOOST ON)
  INCLUDE(${PROJECT_SOURCE_DIR}/deps/stage/boost.build)
  SET(BUILD_EMBEDDED_WT ON)
  INCLUDE(${PROJECT_SOURCE_DIR}/deps/stage/wt.build)
  SET(BUILD_EMBEDDED_FFMPEG ON)
  INCLUDE(${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg.build)
  SET(BUILD_EMBEDDED_X264 ON)
  INCLUDE(${PROJECT_SOURCE_DIR}/deps/stage/x264.build)

# settings for the embedded Library Paths
  SET(BOOST_DIR ${PROJECT_SOURCE_DIR}/deps/stage/boost)
  SET(BOOST_COMPILER gcc41 )
  SET(BOOST_VERSION "1_35" )
#  SET(FFMPEG_DIR ${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg)
  SET(WT_DIR ${PROJECT_SOURCE_DIR}/deps/stage/wt)
  SET(X264_DIR ${PROJECT_SOURCE_DIR}/deps/stage/x264)
ENDIF(BUILD_EMBEDDED_LIBS)

add_subdirectory (src)
add_subdirectory (test)


#INCLUDE(cmake/HiveBuildBoost.txt)
#INCLUDE(cmake/HiveBuildWT.txt)
#INCLUDE(cmake/HiveBuildX264.txt)
#INCLUDE(cmake/HiveBuildFfmpeg.txt)

	



IF(NOT BUILD_EMBEDDED_LIBS)
SET(FFMPEG_DIR ${USERLIB_ROOT} CACHE PATH "Use ffmpeg in directory")
#SET(X264_DIR ${USERLIB_ROOT} CACHE PATH "Use x264 codec in directory")
SET(BOOST_DIR /usr CACHE PATH "Boost installation path prefix")
SET(BOOST_COMPILER gcc CACHE STRING "Boost libraries compiler signature")
SET(BOOST_VERSION "1_35" CACHE STRING "Boost libraries version signature")
SET(MYSQL_DIR ${USERLIB_ROOT} CACHE PATH "Mysql libraries")
SET(WT_DIR ${USERLIB_ROOT} CACHE PATH "WebToolKit libraries")
SET(BREAKPAD_DIR ${USERLIB_ROOT} CACHE PATH "Google Breakpad libraries")
ENDIF(NOT BUILD_EMBEDDED_LIBS)


#LINK_DIRECTORIES(${FFMPEG_DIR}/lib)


ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)


#INCLUDE(cmake/WtFindBoost.txt)

IF(WIN32)
  INSTALL(FILES "c:/vcredist_x86.exe" DESTINATION . )
  SET( ISS_SOURCE_PATH ${CMAKE_INSTALL_PREFIX})
  CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/iss/setup.iss.in ${PROJECT_SOURCE_DIR}/iss/setup.iss)
ENDIF(WIN32)


CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_SOURCE_DIR}/config.h)

INCLUDE(CPack)


