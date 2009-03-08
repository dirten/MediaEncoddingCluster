ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)
ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)


FILE(GLOB ERLANG_INTERFACE_DIR ${ERLANG_DIR}/lib/erl_interface-*)
MESSAGE("Erlang Interface Dir " ${ERLANG_INTERFACE_DIR})
#SET(BZ2_LIB bz2)

INCLUDE_DIRECTORIES (${ERLANG_INTERFACE_DIR}/include)
INCLUDE_DIRECTORIES (${MEC_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES (${BOOST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES (${FFMPEG_DIR} ${FFMPEG_INC_DIR})
INCLUDE_DIRECTORIES (${FFMPEG_DIR}/include)

LINK_DIRECTORIES    (${ERLANG_INTERFACE_DIR}/lib/)
#LINK_DIRECTORIES    (${BOOST_LIB_DIR})
#LINK_DIRECTORIES    (${BOOST_DIR}/lib)
LINK_DIRECTORIES    (${FFMPEG_DIR}/lib)
LINK_DIRECTORIES    (${X264_DIR}/lib)

IF(WIN32)
  SET(RELEASE_DIR "/Release")
  ADD_DEFINITIONS(-D__WIN32__)
ENDIF(WIN32)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/src/erl/bin) 

#SET(FILEPORTEXE ${MEC_BINARY_DIR}/src/erl${RELEASE_DIR}/erltest)
SET(SYSPORTEXE ${MEC_BINARY_DIR}/src/erl${RELEASE_DIR}/mhivesys)
#SET(ENCODECLIENTEXE ${MEC_BINARY_DIR}/src/erl${RELEASE_DIR}/erlclient)
#SET(WRITERPORTEXE ${MEC_BINARY_DIR}/src/erl${RELEASE_DIR}/mhivewriter)

IF(WIN32)
	SET( ISS_SOURCE_PATH ${CMAKE_INSTALL_PREFIX})
	CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/iss/setup.iss.in ${PROJECT_SOURCE_DIR}/iss/setup.iss)
ENDIF(WIN32)

#ADD_EXECUTABLE (erltest erltest.cpp)
#ADD_EXECUTABLE (erlclient erlclient.cpp)
#ADD_EXECUTABLE (mhivewriter mhivewriter.cpp)
ADD_EXECUTABLE (mhivesys erlsys.cpp)


#TARGET_LINK_LIBRARIES (erltest z ${BZ2_LIB} av util io lang avformat avcodec avutil )
#TARGET_LINK_LIBRARIES (erlclient z ${BZ2_LIB} av lang util io avcodec avformat avutil swscale )
#TARGET_LINK_LIBRARIES (mhivewriter z ${BZ2_LIB} av lang util io avcodec avformat avutil swscale )
TARGET_LINK_LIBRARIES (mhivesys z ${BZ2_LIB} av lang util io avcodec avformat avutil swscale )

IF(WIN32)
#  TARGET_LINK_LIBRARIES (erltest z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 a52 x264 erl_interface ei)
#  TARGET_LINK_LIBRARIES (erlclient z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 a52 x264 erl_interface ei)
#  TARGET_LINK_LIBRARIES (mhivewriter z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 a52 x264 erl_interface ei)
  TARGET_LINK_LIBRARIES (mhivesys z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 a52 x264 erl_interface ei)
ELSEIF(APPLE)
#  TARGET_LINK_LIBRARIES (erltest x264 bz2 erl_interface ei pthread)
#  TARGET_LINK_LIBRARIES (erlclient x264 bz2 erl_interface ei  pthread)
#  TARGET_LINK_LIBRARIES (mhivewriter x264 bz2 erl_interface ei  pthread)
  TARGET_LINK_LIBRARIES (mhivesys x264 bz2 erl_interface ei  pthread)
ELSE(WIN32)
#  TARGET_LINK_LIBRARIES (erltest x264  erl_interface ei pthread)
#  TARGET_LINK_LIBRARIES (erlclient x264  erl_interface ei  pthread)
#  TARGET_LINK_LIBRARIES (mhivewriter x264  erl_interface ei  pthread)
  TARGET_LINK_LIBRARIES (mhivesys x264  erl_interface ei  pthread)
ENDIF(WIN32)

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

#target_link_libraries (mhivewriter
#  ${BOOST_SYSTEM_LIB_MT}
#  ${BOOST_FS_LIB_MT}
#)

CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/erl/include/config.hrl.in ${PROJECT_SOURCE_DIR}/src/erl/include/config.hrl)

INSTALL(TARGETS mhivesys
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})

#execute_process(COMMAND ${ERLANG_DIR}/bin/erl -args_file compile.args
#                  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl/
#                  OUTPUT_FILE ${PROJECT_SOURCE_DIR}/src/erl/compile.log 
#                  ERROR_FILE ${PROJECT_SOURCE_DIR}/src/erl/compile.log
#                  )

#file(GLOB ERLANG_SRC_FILES ${PROJECT_SOURCE_DIR}/src/erl/*.erl)
add_custom_target(erlang ALL ${ERLANG_DIR}/bin/erl -args_file compile.args
WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl)

add_custom_target(tsystem ${ERLANG_DIR}/bin/erl -args_file system.args
WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl)

configure_file(mhive.app ${PROJECT_SOURCE_DIR}/src/erl/ebin/mhive.app COPYONLY)
