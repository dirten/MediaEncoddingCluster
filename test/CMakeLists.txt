
#add_subdirectory (org)

include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${FFMPEG_DIR}/include)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${MEC_SOURCE_DIR}/test/fobs/dist/include)

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

INCLUDE_DIRECTORIES (${ERLANG_INTERFACE_DIR}/include)
LINK_DIRECTORIES    (${ERLANG_INTERFACE_DIR}/lib/)

ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)


add_executable (test test.cpp)
add_executable (erltest erltest.cpp)
add_executable (info info.cpp)
IF(WIN32)
target_link_libraries (info libavcodec libavformat libavutil libswscale  ${BZ2_LIB} io av util lang faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 Winmm a52 x264 )
target_link_libraries (test libavcodec libavformat libavutil libswscale z ${BZ2_LIB} io av util lang faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 Winmm a52 x264 )
target_link_libraries (erltest libavcodec libavformat libavutil libswscale z ${BZ2_LIB} io av util lang faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 Winmm a52 x264 )
ELSEIF(APPLE)
target_link_libraries (info bz2 )
target_link_libraries (test bz2 )
target_link_libraries (erltest bz2 )
ELSE(WIN32)

ENDIF(WIN32)

IF(NOT WIN32)

target_link_libraries (info ${BZ2_LIB}  io av util lang avformat avcodec avutil swscale  x264 pthread z mp3lame theora vorbis vorbisenc xvidcore ogg)
target_link_libraries (test ${BZ2_LIB} io av util lang avformat avcodec avutil swscale  x264 pthread z mp3lame theora vorbis vorbisenc xvidcore ogg)
target_link_libraries (erltest ${BZ2_LIB} erl_interface ei io av util lang avformat avcodec avutil swscale  x264 pthread z mp3lame theora vorbis vorbisenc xvidcore ogg)
ENDIF(NOT WIN32)

