project (MEC)



SET(BUILD_SHARED_LIBS OFF)

SET(BUILD_EMBEDDED_LIBS CACHE STRING "Use Embedded Dependency Libraries")



# Build Embedded libraries in deps directory
IF(BUILD_EMBEDDED_LIBS)
# setup to build embedded libraries
  SET(BUILD_SHARED_LIBS ON)
  SET(BUILD_EMBEDDED_BOOST ON)
  SET(BUILD_EMBEDDED_WT ON)
  SET(BUILD_EMBEDDED_FFMPEG ON)
  SET(BUILD_EMBEDDED_X264 ON)

# settings for the embedded Library Paths
  SET(BOOST_DIR ${PROJECT_SOURCE_DIR}/deps/stage/boost CACHE PATH "Boost installation path prefix")
  SET(BOOST_COMPILER gcc41 CACHE STRING "Boost libraries compiler signature")
  SET(BOOST_VERSION "1_35" CACHE STRING "Boost libraries version signature")
  SET(FFMPEG_DIR ${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg CACHE PATH "Use ffmpeg in directory")
  SET(WT_DIR ${PROJECT_SOURCE_DIR}/deps/stage/wt CACHE PATH "Use WT in directory")
  SET(X264_DIR ${PROJECT_SOURCE_DIR}/deps/stage/x264 CACHE PATH "Use x264 codec in directory")
ENDIF(BUILD_EMBEDDED_LIBS)

add_subdirectory (src)
#add_subdirectory (test)



FIND_PATH(BOOST_JAM_DIR
    build_dist.sh
  PATHS
    ${PROJECT_SOURCE_DIR}/deps/boost_1_35_0/tools/jam
  NO_DEFAULT_PATH
)

#include(${PROJECT_SOURCE_DIR}/deps/wt-2.1.5/CMakeList.txt)

#MESSAGE("** BOOST_EMBEDDED " ${BOOST_JAM_DIR} ".")





IF(BUILD_EMBEDDED_BOOST)
execute_process(COMMAND ${BOOST_JAM_DIR}/build_dist.sh
                WORKING_DIRECTORY ${BOOST_JAM_DIR})

execute_process(COMMAND ${BOOST_JAM_DIR}/stage/bin.linuxx86/bjam --prefix=${PROJECT_SOURCE_DIR}/deps/stage/boost
                variant=release threading=multi link=static --without-python --without-wave --without-graph --without-test --without-mpi install
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/boost_1_35_0)
ENDIF(BUILD_EMBEDDED_BOOST)


IF(BUILD_EMBEDDED_WT)
#FILE(GLOB_RECURSE BOOST_DT_LIB_MT ${PROJECT_SOURCE_DIR}/deps/stage/*/libboost_date_time*)
#FILE(GLOB_RECURSE BOOST_FS_LIB_MT ${PROJECT_SOURCE_DIR}/deps/stage/*/libboost_filesystem*)
#FILE(GLOB_RECURSE BOOST_PO_LIB_MT ${PROJECT_SOURCE_DIR}/deps/stage/*/libboost_program_options*)
#FILE(GLOB_RECURSE BOOST_REGEX_LIB_MT ${PROJECT_SOURCE_DIR}/deps/stage/*/libboost_regex*)
#FILE(GLOB_RECURSE BOOST_SIGNAL_LIB_MT ${PROJECT_SOURCE_DIR}/deps/stage/*/libboost_signal*)
#FILE(GLOB_RECURSE BOOST_SYSTEM_LIB_MT ${PROJECT_SOURCE_DIR}/deps/stage/*/libboost_system*)
#FILE(GLOB_RECURSE BOOST_THREAD_LIB_MT ${PROJECT_SOURCE_DIR}/deps/stage/*/libboost_thread*)
#MESSAGE("** BOOST_EMBEDDED_DATE_TIME " ${BOOST_DT_LIB_MT} )
#MESSAGE("** BOOST_EMBEDDED_FILESYSTEM " ${BOOST_FS_LIB_MT} )
#MESSAGE("** BOOST_EMBEDDED_PROGRAM_OPTION " ${BOOST_PO_LIB_MT} )
#MESSAGE("** BOOST_EMBEDDED_REGEX " ${BOOST_REGEX_LIB_MT} )

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/stage/wt)

execute_process(COMMAND cmake 
                -DBOOST_DIR=${PROJECT_SOURCE_DIR}/deps/stage/boost
                -DBOOST_VERSION=1_35
                -DBOOST_COMPILER=gcc41
                -DSHARED_LIBS=OFF
                -DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR}/deps/stage/wt
                ../../wt-2.1.5
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/stage/wt)

execute_process(COMMAND make install WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/stage/wt)

ENDIF(BUILD_EMBEDDED_WT)

IF(BUILD_EMBEDDED_X264)
  execute_process(COMMAND ./configure 
                --prefix=${PROJECT_SOURCE_DIR}/deps/stage/x264
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/x264)
  execute_process(COMMAND make install WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/x264)
ENDIF(BUILD_EMBEDDED_X264)

IF(BUILD_EMBEDDED_FFMPEG)
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg)
execute_process(COMMAND  ../../ffmpeg/configure 
                --prefix=${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg
                --enable-swscale
                --enable-gpl
                --disable-shared
                --enable-static
                --disable-stripping
                --enable-libx264
                --extra-ldflags=-L${PROJECT_SOURCE_DIR}/deps/stage/x264/lib
                --extra-ldflags=-lpthread
                --extra-cflags=-I${PROJECT_SOURCE_DIR}/deps/stage/x264/include
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg)
execute_process(COMMAND make WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg)
execute_process(COMMAND make install WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg)
ENDIF(BUILD_EMBEDDED_FFMPEG)
#                --extra-ldflags=-L${PROJECT_SOURCE_DIR}/deps/stage/x264/lib


#FILE(GLOB_RECURSE SIGNAL ${PROJECT_SOURCE_DIR}/deps/stage/*/libboost_signals*)
#MESSAGE("** BOOST_EMBEDDED_SIGNAL " ${SIGNAL} )

#execute_process()

SET(FFMPEG_DIR ${USERLIB_ROOT} CACHE PATH "Use ffmpeg in directory")
#SET(FFMPEG_LIB_DIR ${USERLIB_ROOT} CACHE PATH "Use ffmpeg Librbaries in directory")
#SET(FFMPEG_INC_DIR ${USERLIB_ROOT} CACHE PATH "Use ffmpeg Includes in directory")
SET(MYSQL_INC_DIR ${USERLIB_ROOT} CACHE PATH "Use mysql Includes in directory")
SET(MYSQL_LIB_DIR ${USERLIB_ROOT} CACHE PATH "Use mysql Librbaries in directory")
#SET(MYSQL_VERSION  CACHE STRING "Use mysql Librbaries Version")
#ADD_DEFINITIONS(-DMYSQL_VERSION=${MYSQL_VERSION})
SET(X264_DIR ${USERLIB_ROOT} CACHE PATH "Use x264 codec in directory")
SET(WT_DIR ${USERLIB_ROOT} CACHE PATH "Use WT in directory")

SET(BOOST_DIR /usr CACHE PATH "Boost installation path prefix")
SET(BOOST_COMPILER gcc CACHE STRING "Boost libraries compiler signature")
SET(BOOST_VERSION "1_34_1" CACHE STRING "Boost libraries version signature")

#IF(NOT BOOST_WT_FOUND)
#  MESSAGE("** Error finding Wt required library: ")
#  MESSAGE("** Could not find a boost installation in " ${BOOST_DIR} ".")
#  MESSAGE("** It may be necessary to set appropriate values for the")
#  MESSAGE("   variables BOOST_DIR, BOOST_COMPILER, and BOOST_VERSION")
#  MESSAGE(FATAL_ERROR "** Wt requires the following C++ boost libraries: boost_date_time, boost_regex, and boost_program_options, boost_signals, and optionally boost_thread")
#ENDIF(NOT BOOST_WT_FOUND)


INCLUDE(cmake/WtFindBoost.txt)
INCLUDE(cmake/WtFindAsio.txt)

