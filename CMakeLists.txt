project (MEC)

SET(BUILD_SHARED_LIBS OFF)

add_subdirectory (src)
add_subdirectory (test)

SET(FFMPEG_DIR ${USERLIB_ROOT} CACHE PATH "Use ffmpeg in directory")
SET(FFMPEG_LIB_DIR ${USERLIB_ROOT} CACHE PATH "Use ffmpeg Librbaries in directory")
SET(FFMPEG_INC_DIR ${USERLIB_ROOT} CACHE PATH "Use ffmpeg Includes in directory")
SET(MYSQL_INC_DIR ${USERLIB_ROOT} CACHE PATH "Use mysql Includes in directory")
SET(MYSQL_LIB_DIR ${USERLIB_ROOT} CACHE PATH "Use mysql Librbaries in directory")
SET(MYSQL_VERSION  CACHE STRING "Use mysql Librbaries Version")
ADD_DEFINITIONS(-DMYSQL_VERSION=${MYSQL_VERSION})
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

