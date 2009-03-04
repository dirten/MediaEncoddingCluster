project (MEC)
INCLUDE(cmake/WtFindBoost.txt)

SET(MAJOR_VERSION 0)
SET(MINOR_VERSION 0)
SET(PATCH_VERSION 4)
SET(MHIVE_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

ADD_DEFINITIONS(-DMHIVE_VERSION)

#SET(BZ2_LIB bz2)

SET(BUILD_SHARED_LIBS OFF)
SET(BUILD_EMBEDDED_LIBS OFF CACHE STRING "Use Embedded Dependency Libraries")
SET(USE_EMBEDDED_MYSQL OFF CACHE STRING "Use Embedded Mysql")

IF(USE_EMBEDDED_MYSQL)
  ADD_DEFINITIONS(-DUSE_EMBEDDED_MYSQL)
ENDIF(USE_EMBEDDED_MYSQL)

# Build Embedded libraries in deps directory
IF(BUILD_EMBEDDED_LIBS)
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


INCLUDE(cmake/HiveBuildBoost.txt)
INCLUDE(cmake/HiveBuildWT.txt)
INCLUDE(cmake/HiveBuildX264.txt)
INCLUDE(cmake/HiveBuildFfmpeg.txt)

	



IF(NOT BUILD_EMBEDDED_LIBS)
SET(ERLANG_DIR ${USERLIB_ROOT} CACHE PATH "Use Erlang in directory")
SET(FFMPEG_DIR ${USERLIB_ROOT} CACHE PATH "Use ffmpeg in directory")
SET(MYSQL_INC_DIR ${USERLIB_ROOT} CACHE PATH "Use mysql Includes in directory")
SET(MYSQL_LIB_DIR ${USERLIB_ROOT} CACHE PATH "Use mysql Librbaries in directory")
SET(MYSQL_DIR ${USERLIB_ROOT} CACHE PATH "Use mysql Librbaries in directory")
SET(X264_DIR ${USERLIB_ROOT} CACHE PATH "Use x264 codec in directory")
SET(WT_DIR ${USERLIB_ROOT} CACHE PATH "Use WT in directory")
SET(BOOST_DIR /usr CACHE PATH "Boost installation path prefix")
SET(BOOST_COMPILER gcc CACHE STRING "Boost libraries compiler signature")
SET(BOOST_VERSION "1_35" CACHE STRING "Boost libraries version signature")
ENDIF(NOT BUILD_EMBEDDED_LIBS)

ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)

IF(WIN32)
SET( ISS_SOURCE_PATH ${CMAKE_INSTALL_PREFIX})
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/iss/setup.iss.in ${PROJECT_SOURCE_DIR}/iss/setup.iss)
ENDIF(WIN32)
#INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/web DESTINATION ${CMAKE_INSTALL_PREFIX} PATTERN ".svn" EXCLUDE)
#INSTALL(FILES ${PROJECT_SOURCE_DIR}/sql/hive-0.0.1.sql DESTINATION ${CMAKE_INSTALL_PREFIX}/sql )
#INSTALL(FILES ${PROJECT_SOURCE_DIR}/setup.xml DESTINATION ${CMAKE_INSTALL_PREFIX}/res )
#INSTALL(FILES ${PROJECT_SOURCE_DIR}/messages.xml DESTINATION ${CMAKE_INSTALL_PREFIX}/res )
#INSTALL(FILES ${MYSQL_DIR}/share/english/errmsg.sys DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
