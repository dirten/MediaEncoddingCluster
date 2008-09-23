project (MEC)

add_subdirectory (src)
add_subdirectory (test)

SET(FFMPEG_DIR ${USERLIB_ROOT} CACHE PATH "Use ffmpeg in directory")
SET(FFMPEG_LIB_DIR ${USERLIB_ROOT} CACHE PATH "Use ffmpeg Librbaries in directory")
SET(FFMPEG_INC_DIR ${USERLIB_ROOT} CACHE PATH "Use ffmpeg Includes in directory")
SET(MYSQL_INC_DIR ${USERLIB_ROOT} CACHE PATH "Use mysql Includes in directory")
SET(MYSQL_LIB_DIR ${USERLIB_ROOT} CACHE PATH "Use mysql Librbaries in directory")

SET(X264_DIR ${USERLIB_ROOT} CACHE PATH "Use x264 codec in directory")
SET(WT_DIR ${USERLIB_ROOT} CACHE PATH "Use WT in directory")

SET(BOOST_DIR /usr CACHE PATH "Boost installation path prefix")
SET(BOOST_COMPILER gcc CACHE STRING "Boost libraries compiler signature")
SET(BOOST_VERSION "1_34_1" CACHE STRING "Boost libraries version signature")


INCLUDE(cmake/WtFindBoost.txt)
INCLUDE(cmake/WtFindAsio.txt)
