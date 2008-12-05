project (MEC)
INCLUDE(cmake/WtFindBoost.txt)
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
  SET(FFMPEG_DIR ${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg)
  SET(WT_DIR ${PROJECT_SOURCE_DIR}/deps/stage/wt)
  SET(X264_DIR ${PROJECT_SOURCE_DIR}/deps/stage/x264)
ENDIF(BUILD_EMBEDDED_LIBS)

add_subdirectory (src)
add_subdirectory (test)



FIND_PATH(BOOST_JAM_DIR
    build_dist.sh
  PATHS
    ${PROJECT_SOURCE_DIR}/deps/boost_1_35_0/tools/jam
  NO_DEFAULT_PATH
)

IF(BUILD_EMBEDDED_BOOST)
  IF(WIN32)
    SET(BUILD_JAM_CMD "build_dist.bat")
  ELSE(WIN32)
    SET(BUILD_JAM_CMD "build_dist.sh")
  ENDIF(WIN32)
  MESSAGE("** Building Boost Jam")
#  FILE(DOWNLOAD http://dfn.dl.sourceforge.net/sourceforge/boost/boost_1_35_0.tar.gz ${PROJECT_SOURCE_DIR}/deps/boost_1_35_0.tar.gz)
  execute_process(COMMAND ${BOOST_JAM_DIR}/${BUILD_JAM_CMD} 
                  WORKING_DIRECTORY ${BOOST_JAM_DIR} 
                  OUTPUT_FILE ${PROJECT_SOURCE_DIR}/deps/jam.log 
                  ERROR_FILE ${PROJECT_SOURCE_DIR}/deps/jam.log)
  MESSAGE("** Building Boost Library")

  execute_process(COMMAND ${BOOST_JAM_DIR}/stage/bin.linuxx86/bjam 
                --prefix=${PROJECT_SOURCE_DIR}/deps/stage/boost
                --layout=system
                variant=release threading=multi link=static --without-python --without-wave --without-graph --without-test --without-mpi install
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/boost_1_35_0
                OUTPUT_FILE ${PROJECT_SOURCE_DIR}/deps/boost.log 
                ERROR_FILE ${PROJECT_SOURCE_DIR}/deps/boost.log)
  file(WRITE ${PROJECT_SOURCE_DIR}/deps/stage/boost.build "SET(BUILD_EMBEDDED_BOOST OFF)")
ENDIF(BUILD_EMBEDDED_BOOST)


IF(BUILD_EMBEDDED_WT)
  file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/stage/wt)
  MESSAGE("** Configure Webtoolkit")

  execute_process(COMMAND cmake 
                -DBOOST_DIR=${PROJECT_SOURCE_DIR}/deps/stage/boost
                -DBOOST_VERSION=1_35
                -DBOOST_COMPILER=gcc41
                -DSHARED_LIBS=OFF
                -DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR}/deps/stage/wt
                ../../wt-2.1.5
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/stage/wt
                OUTPUT_FILE ${PROJECT_SOURCE_DIR}/deps/wt-configure.log 
                ERROR_FILE ${PROJECT_SOURCE_DIR}/deps/wt-configure.log)

  MESSAGE("** Building Webtoolkit")

  execute_process(COMMAND make install WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/stage/wt
                  OUTPUT_FILE ${PROJECT_SOURCE_DIR}/deps/wt-compile.log 
                  ERROR_FILE ${PROJECT_SOURCE_DIR}/deps/wt-compile.log)

  file(WRITE ${PROJECT_SOURCE_DIR}/deps/stage/wt.build "SET(BUILD_EMBEDDED_WT OFF)")
ENDIF(BUILD_EMBEDDED_WT)

IF(BUILD_EMBEDDED_X264)
  MESSAGE("** Configure x264")

  execute_process(COMMAND ./configure 
                --prefix=${PROJECT_SOURCE_DIR}/deps/stage/x264
                --disable-asm
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/x264
                OUTPUT_FILE ${PROJECT_SOURCE_DIR}/deps/x264-configure.log 
                ERROR_FILE ${PROJECT_SOURCE_DIR}/deps/x264-configure.log)

  MESSAGE("** Build and Install x264")
  execute_process(COMMAND make install WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/x264
                  OUTPUT_FILE ${PROJECT_SOURCE_DIR}/deps/x264-compile.log 
                  ERROR_FILE ${PROJECT_SOURCE_DIR}/deps/x264-compile.log)

  file(WRITE ${PROJECT_SOURCE_DIR}/deps/stage/x264.build "SET(BUILD_EMBEDDED_X264 OFF)")
ENDIF(BUILD_EMBEDDED_X264)

IF(BUILD_EMBEDDED_FFMPEG)
  file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg)
  MESSAGE("** Configure ffmpeg")
  execute_process(COMMAND  ../../ffmpeg/configure 
                --prefix=${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg
                --enable-swscale
                --enable-gpl
                --disable-shared
                --enable-static
                --disable-stripping
                --disable-devices
                --enable-libx264
                --extra-ldflags=-L${PROJECT_SOURCE_DIR}/deps/stage/x264/lib
                --extra-ldflags=-lpthread
                --extra-cflags=-I${PROJECT_SOURCE_DIR}/deps/stage/x264/include
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg
                OUTPUT_FILE ${PROJECT_SOURCE_DIR}/deps/ffmpeg-configure.log 
                ERROR_FILE ${PROJECT_SOURCE_DIR}/deps/ffmpeg-configure.log)

  MESSAGE("** Build ffmpeg")
  execute_process(COMMAND make WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg
                  OUTPUT_FILE ${PROJECT_SOURCE_DIR}/deps/ffmpeg-compile.log 
                  ERROR_FILE ${PROJECT_SOURCE_DIR}/deps/ffmpeg-compile.log)

  MESSAGE("** Install ffmpeg")
  execute_process(COMMAND make install WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg
                  OUTPUT_FILE ${PROJECT_SOURCE_DIR}/deps/ffmpeg-install.log 
                  ERROR_FILE ${PROJECT_SOURCE_DIR}/deps/ffmpeg-install.log)

  file(WRITE ${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg.build "SET(BUILD_EMBEDDED_FFMPEG OFF)")
ENDIF(BUILD_EMBEDDED_FFMPEG)


IF(NOT BUILD_EMBEDDED_LIBS)
SET(FFMPEG_DIR ${USERLIB_ROOT} CACHE PATH "Use ffmpeg in directory")
#SET(MYSQL_INC_DIR ${USERLIB_ROOT} CACHE PATH "Use mysql Includes in directory")
#SET(MYSQL_LIB_DIR ${USERLIB_ROOT} CACHE PATH "Use mysql Librbaries in directory")
SET(MYSQL_DIR ${USERLIB_ROOT} CACHE PATH "Use mysql Librbaries in directory")
SET(X264_DIR ${USERLIB_ROOT} CACHE PATH "Use x264 codec in directory")
SET(WT_DIR ${USERLIB_ROOT} CACHE PATH "Use WT in directory")
SET(BOOST_DIR /usr CACHE PATH "Boost installation path prefix")
SET(BOOST_COMPILER gcc CACHE STRING "Boost libraries compiler signature")
SET(BOOST_VERSION "1_35" CACHE STRING "Boost libraries version signature")
ENDIF(NOT BUILD_EMBEDDED_LIBS)

ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
#IF(NOT BOOST_WT_FOUND)
#  MESSAGE("** Error finding Wt required library: ")
#  MESSAGE("** Could not find a boost installation in " ${BOOST_DIR} ".")
#  MESSAGE("** It may be necessary to set appropriate values for the")
#  MESSAGE("   variables BOOST_DIR, BOOST_COMPILER, and BOOST_VERSION")
#  MESSAGE(FATAL_ERROR "** Wt requires the following C++ boost libraries: boost_date_time, boost_regex, and boost_program_options, boost_signals, and optionally boost_thread")
#ENDIF(NOT BOOST_WT_FOUND)
IF(WIN32)
SET( ISS_SOURCE_PATH ${CMAKE_INSTALL_PREFIX})
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/iss/setup.iss.in ${PROJECT_SOURCE_DIR}/iss/setup2.iss)
ENDIF(WIN32)
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/web DESTINATION ${CMAKE_INSTALL_PREFIX} PATTERN ".svn" EXCLUDE)
INSTALL(FILES ${PROJECT_SOURCE_DIR}/sql/hive-0.0.1.sql DESTINATION ${CMAKE_INSTALL_PREFIX}/sql )
INSTALL(FILES ${PROJECT_SOURCE_DIR}/setup.xml DESTINATION ${CMAKE_INSTALL_PREFIX}/res )
INSTALL(FILES ${PROJECT_SOURCE_DIR}/messages.xml DESTINATION ${CMAKE_INSTALL_PREFIX}/res )
INSTALL(FILES ${MYSQL_DIR}/share/english/errmsg.sys DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
