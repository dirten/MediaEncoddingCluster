
ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)
ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_DIR} ${FFMPEG_INC_DIR})
include_directories (${FFMPEG_DIR}/include)
include_directories (/usr/lib/erlang/lib/erl_interface-3.5.5.4/include/)
LINK_DIRECTORIES(${BOOST_LIB_DIR})
LINK_DIRECTORIES(${BOOST_DIR}/lib)
LINK_DIRECTORIES(${FFMPEG_DIR}/lib)

LINK_DIRECTORIES(${X264_DIR}/lib)
LINK_DIRECTORIES(/usr/lib/erlang/lib/erl_interface-3.5.5.4/lib/)

FIND_LIBRARY(AVCODEC_LIB
    NAMES
      avcodec
      libavcodec
    PATHS
      ${FFMPEG_DIR}/lib
    NO_DEFAULT_PATH
  )
FIND_LIBRARY(AVFORMAT_LIB
    NAMES
      avformat
      libavformat
    PATHS
      ${FFMPEG_DIR}/lib
    NO_DEFAULT_PATH
  )
#FIND_LIBRARY(avc avcodec)

#MESSAGE("** AVCODEC PATH: " ${AVCODEC_LIB})
#MESSAGE("** BOOST_LIB PATH: " ${BOOST_LIB_DIR})


add_executable (erltest erltest.cpp)


target_link_libraries (erltest esb sql lang av util io avcodec avformat avutil swscale )
IF(WIN32)
#target_link_libraries (mhive z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 Winmm a52 x264 ${MYSQL_LIB} wtd wtextd wthttpd)
target_link_libraries (erltest x264 Winmm ${MYSQL_LIB} wtd wtextd wthttpd )
ELSE(WIN32)
target_link_libraries (erltest x264 erl_interface ei pthread)
ENDIF(WIN32)

target_link_libraries (erltest
  ${BOOST_SERIAL_LIB_MT} 
  ${BOOST_THREAD_LIB_MT} 
  ${BOOST_SYSTEM_LIB_MT} 
  ${BOOST_PO_LIB_MT} 
  ${BOOST_SIGNALS_LIB_MT} 
  ${BOOST_REGEX_LIB_MT} 
  ${BOOST_FS_LIB_MT}
  ${BOOST_DT_LIB_MT}
)



INSTALL(TARGETS erltest
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
