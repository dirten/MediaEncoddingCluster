
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/erl/config.hrl.in ${PROJECT_SOURCE_DIR}/src/erl/config.hrl)

ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)
ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_DIR} ${FFMPEG_INC_DIR})
include_directories (${FFMPEG_DIR}/include)
IF(WIN32)
SET(FILEPORTEXE ${MEC_BINARY_DIR}/src/erl/Debug/erltest)
SET(ENCODECLIENTEXE ${MEC_BINARY_DIR}/src/erl/Debug/erlclient)
ADD_DEFINITIONS(-D__WIN32__)
include_directories (${ERLANG_DIR}/lib/erl_interface-3.5.9/include)
LINK_DIRECTORIES(${ERLANG_DIR}/lib/erl_interface-3.5.9/lib/)
ELSE(WIN32)
SET(FILEPORTEXE ${PROJECT_SOURCE_DIR}/build/src/erl/erltest)
SET(ENCODECLIENTEXE ${PROJECT_SOURCE_DIR}/build/src/erl/erlclient)
include_directories (${ERLANG_DIR}/lib/erl_interface-3.5.9/include/)
LINK_DIRECTORIES(${ERLANG_DIR}/lib/erl_interface-3.5.9/lib/)
ENDIF(WIN32)
LINK_DIRECTORIES(${BOOST_LIB_DIR})
LINK_DIRECTORIES(${BOOST_DIR}/lib)
LINK_DIRECTORIES(${FFMPEG_DIR}/lib)

LINK_DIRECTORIES(${X264_DIR}/lib)

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
add_executable (erlclient erlclient.cpp)
add_executable (mhivewriter mhivewriter.cpp)


target_link_libraries (erltest z bz2 av hive util io lang avformat avcodec avutil )
target_link_libraries (erlclient z bz2 esb sql av lang util io hive avcodec avformat avutil swscale )
target_link_libraries (mhivewriter z bz2 esb sql av lang util io hive avcodec avformat avutil swscale )
#target_link_libraries (erltest z esb sql av lang util io hive avcodec avformat avutil swscale )
IF(WIN32)
#target_link_libraries (erltest z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 Winmm a52 x264 erl_interface ei)
target_link_libraries (erltest z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 a52 x264 erl_interface ei)
target_link_libraries (erlclient z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 a52 x264 erl_interface ei)
target_link_libraries (mhivewriter z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 a52 x264 erl_interface ei)
#target_link_libraries (erltest x264 Winmm erl_interface ei )
ELSE(WIN32)
target_link_libraries (erltest x264  erl_interface ei pthread)
target_link_libraries (erlclient x264  erl_interface ei  pthread)
target_link_libraries (mhivewriter x264  erl_interface ei  pthread)
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
target_link_libraries (mhivewriter
  ${BOOST_SYSTEM_LIB_MT}
  ${BOOST_FS_LIB_MT}
)


CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/erl/config.hrl.in ${PROJECT_SOURCE_DIR}/src/erl/config.hrl)

INSTALL(TARGETS erltest
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})

file(GLOB ERLANG_SRC_FILES ${PROJECT_SOURCE_DIR}/src/erl/*.erl)
MESSAGE("** ERLANG SOURCES: " ${ERLANG_SRC_FILES})
add_custom_target(erlang ALL erlc ${ERLANG_SRC_FILES}
WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl)
