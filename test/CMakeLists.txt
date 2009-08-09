
#ENABLE_TESTING()
#ADD_TEST(FIRSTTEST ${EXECUTABLE_OUTPUT_PATH}/info "/media/out/ChocolateFactory.mp2")


include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${FFMPEG_DIR}/include)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${MEC_SOURCE_DIR}/test/fobs/dist/include)
INCLUDE_DIRECTORIES (${ERLANG_INTERFACE_DIR}/include)

LINK_DIRECTORIES(${BOOST_LIB_DIR})
LINK_DIRECTORIES(${BOOST_DIR}/lib)
LINK_DIRECTORIES(${FFMPEG_DIR}/lib)

LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/test/fobs/dist/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libogg/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/x264/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/xvidcore/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/lame/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libtheora/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libvorbis/lib)
add_subdirectory (org)


FILE(GLOB ERLANG_INTERFACE_DIR ${ERLANG_DIR}/lib/erl_interface-*)
MESSAGE("Erlang Interface Dir for test" ${ERLANG_INTERFACE_DIR})


FIND_LIBRARY(ERL_INTERFACE_LIB
    NAMES
      erl_interface
    PATHS
      ${ERLANG_INTERFACE_DIR}/lib/
    NO_DEFAULT_PATH
  )

FIND_LIBRARY(ERL_EI_LIB
    NAMES
      ei
    PATHS
      ${ERLANG_INTERFACE_DIR}/lib/
    NO_DEFAULT_PATH
  )
IF(WIN32)
  ADD_DEFINITIONS(-D__WIN32__)
ENDIF(WIN32)


INCLUDE_DIRECTORIES (${ERLANG_INTERFACE_DIR}/include)
LINK_DIRECTORIES    (${ERLANG_INTERFACE_DIR}/lib/)

ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)


add_executable (test test.cpp)
#IF(NOT WIN32)
#  add_executable (erltest erltest.cpp)
#  target_link_libraries (erltest avcodec avformat avutil swscale z ${BZ2_LIB} io av util lang faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 Winmm a52 x264 )
#ENDIF(NOT WIN32)

add_executable (info info.cpp)
target_link_libraries (info  io av util lang avformat avcodec avutil swscale)


IF(WIN32)
#  TARGET_LINK_LIBRARIES (info erl_interface ei libx264 wsock32 vfw32 winmm libmp3lame libtheora libvorbis libvorbisenc xvidcore libogg)
#  TARGET_LINK_LIBRARIES (info erl_interface ei  libgcc libmingwex libcoldname libmoldname90 wsock32 vfw32 winmm )
#target_link_libraries (info  )

#target_link_libraries (info ${BZ2_LIB} io av util lang avcodec avformat avutil swscale   )
#target_link_libraries (test avcodec avformat avutil swscale ${BZ2_LIB} io av util lang libx264 wsock32 vfw32 winmm libmp3lame libtheora libvorbis libvorbisenc xvidcore libogg )
ELSEIF(APPLE)
target_link_libraries (info x264 ${MYSQL_LIB} z pthread z mp3lame theora vorbis vorbisenc xvidcore ogg)
target_link_libraries (info bz2 )
target_link_libraries (test bz2 )
#target_link_libraries (erltest bz2 )
ELSE(WIN32)
target_link_libraries (info x264 ${MYSQL_LIB} z pthread z mp3lame theora vorbis vorbisenc xvidcore ogg)

ENDIF(WIN32)
target_link_libraries (info
  ${BOOST_SERIAL_LIB_MT}
  ${BOOST_THREAD_LIB_MT}
  ${BOOST_SYSTEM_LIB_MT}
  ${BOOST_PO_LIB_MT}
  ${BOOST_SIGNALS_LIB_MT}
  ${BOOST_REGEX_LIB_MT}
  ${BOOST_FS_LIB_MT}
  ${BOOST_DT_LIB_MT}
)
#target_link_libraries (erltest
#  ${BOOST_SERIAL_LIB_MT}
#  ${BOOST_THREAD_LIB_MT}
#  ${BOOST_SYSTEM_LIB_MT}
#  ${BOOST_PO_LIB_MT}
#  ${BOOST_SIGNALS_LIB_MT}
#  ${BOOST_REGEX_LIB_MT}
#  ${BOOST_FS_LIB_MT}
#  ${BOOST_DT_LIB_MT}
#)

MESSAGE("BOOST FileSystem Library" ${BOOST_FS_LIB_MT})


IF(NOT WIN32)

target_link_libraries (info ${BZ2_LIB} erl_interface ei io av util lang avformat avcodec avutil swscale  x264 pthread z mp3lame theora vorbis vorbisenc xvidcore ogg)
target_link_libraries (test ${BZ2_LIB} io av util lang avformat avcodec avutil swscale  x264 pthread z mp3lame theora vorbis vorbisenc xvidcore ogg)
#target_link_libraries (erltest ${BZ2_LIB} erl_interface ei io av util lang avformat avcodec avutil swscale  x264 pthread z mp3lame theora vorbis vorbisenc xvidcore ogg)
ENDIF(NOT WIN32)

