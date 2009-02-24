ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)
ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)


FILE(GLOB ERLANG_INTERFACE_DIR ${ERLANG_DIR}/lib/erl_interface-*)

#SET(BZ2_LIB bz2)

INCLUDE_DIRECTORIES (${ERLANG_INTERFACE_DIR}/include)
INCLUDE_DIRECTORIES (${MEC_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES (${BOOST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES (${FFMPEG_DIR} ${FFMPEG_INC_DIR})
INCLUDE_DIRECTORIES (${FFMPEG_DIR}/include)

LINK_DIRECTORIES    (${ERLANG_INTERFACE_DIR}/lib/)
LINK_DIRECTORIES    (${BOOST_LIB_DIR})
LINK_DIRECTORIES    (${BOOST_DIR}/lib)
LINK_DIRECTORIES    (${FFMPEG_DIR}/lib)
LINK_DIRECTORIES    (${X264_DIR}/lib)

IF(WIN32)
  SET(FILEPORTEXE ${MEC_BINARY_DIR}/src/erl/Debug/erltest)
  SET(ENCODECLIENTEXE ${MEC_BINARY_DIR}/src/erl/Debug/erlclient)
  ADD_DEFINITIONS(-D__WIN32__)
ELSE(WIN32)
  SET(FILEPORTEXE ${PROJECT_SOURCE_DIR}/build/src/erl/erltest)
  SET(ENCODECLIENTEXE ${PROJECT_SOURCE_DIR}/build/src/erl/erlclient)
ENDIF(WIN32)

ADD_EXECUTABLE (erltest erltest.cpp)
ADD_EXECUTABLE (erlclient erlclient.cpp)
ADD_EXECUTABLE (mhivewriter mhivewriter.cpp)


TARGET_LINK_LIBRARIES (erltest z ${BZ2_LIB} av hive util io lang avformat avcodec avutil )
TARGET_LINK_LIBRARIES (erlclient z ${BZ2_LIB} esb sql av lang util io hive avcodec avformat avutil swscale )
TARGET_LINK_LIBRARIES (mhivewriter z ${BZ2_LIB} esb sql av lang util io hive avcodec avformat avutil swscale )

IF(WIN32)
  TARGET_LINK_LIBRARIES (erltest z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 a52 x264 erl_interface ei)
  TARGET_LINK_LIBRARIES (erlclient z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 a52 x264 erl_interface ei)
  TARGET_LINK_LIBRARIES (mhivewriter z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 a52 x264 erl_interface ei)
ELSEIF(APPLE)
  TARGET_LINK_LIBRARIES (erltest x264 bz2 erl_interface ei pthread)
  TARGET_LINK_LIBRARIES (erlclient x264 bz2 erl_interface ei  pthread)
  TARGET_LINK_LIBRARIES (mhivewriter x264 bz2 erl_interface ei  pthread)
ELSE(WIN32)
  TARGET_LINK_LIBRARIES (erltest x264  erl_interface ei pthread)
  TARGET_LINK_LIBRARIES (erlclient x264  erl_interface ei  pthread)
  TARGET_LINK_LIBRARIES (mhivewriter x264  erl_interface ei  pthread)
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
add_custom_target(erlang ALL ${ERLANG_DIR}/bin/erlc -Initrogen/include ${ERLANG_SRC_FILES}
WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl)
