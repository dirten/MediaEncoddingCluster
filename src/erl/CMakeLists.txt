ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)

#ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
#ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)



#ADD_DEFINITIONS(-NODEFAULTLIB LIBCMT)

FILE(GLOB ERLANG_INTERFACE_DIR ${ERLANG_DIR}/lib/erl_interface-*)
MESSAGE("Erlang Interface Dir " ${ERLANG_INTERFACE_DIR})


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
#MESSAGE("Erlang Interface LIB " ${ERL_INTERFACE_LIB})
#MESSAGE("Erlang Interface LIB " ${ERL_EI_LIB})

#SET(BZ2_LIB bz2)

INCLUDE_DIRECTORIES (${ERLANG_INTERFACE_DIR}/include)
INCLUDE_DIRECTORIES (${MEC_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES (${BOOST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES (${FFMPEG_DIR}/include)


LINK_DIRECTORIES    (${ERLANG_INTERFACE_DIR}/lib/)
LINK_DIRECTORIES    (${BOOST_LIB_DIR})
LINK_DIRECTORIES    (${BOOST_DIR}/lib)
LINK_DIRECTORIES    (${FFMPEG_DIR}/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libogg/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/x264/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/xvidcore/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/lame/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libtheora/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libvorbis/lib)
#LINK_DIRECTORIES    (${X264_DIR}/lib)
#MESSAGE("** FFMPEG PATH: " ${MEC_SOURCE_DIR})


MESSAGE("** AVCODEC PATH: " ${AVCODEC_LIB})

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

IF(WIN32)
  SET(RELEASE_DIR "/Release")
  ADD_DEFINITIONS(-D__WIN32__)
  #ADD_DEFINITIONS(-MTd)
  SET_TARGET_PROPERTIES(mhivesys PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libcmt")

#  ADD_DEFINITIONS("/NODEFAULTLIB:\"LIBCMT\"" ) 
ENDIF(WIN32)


#TARGET_LINK_LIBRARIES (erltest z ${BZ2_LIB} av util io lang avformat avcodec avutil )
#TARGET_LINK_LIBRARIES (erlclient z ${BZ2_LIB} av lang util io avcodec avformat avutil swscale )
#TARGET_LINK_LIBRARIES (mhivewriter z ${BZ2_LIB} av lang util io avcodec avformat avutil swscale )


IF(WIN32)
	MESSAGE("** Link Windows: " )
#  TARGET_LINK_LIBRARIES (erltest z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 a52 x264 erl_interface ei)
#  TARGET_LINK_LIBRARIES (erlclient z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 a52 x264 erl_interface ei)
#  TARGET_LINK_LIBRARIES (mhivewriter z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 a52 x264 erl_interface ei)
#  TARGET_LINK_LIBRARIES (mhivesys z faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 a52 x264 erl_interface ei)
##  TARGET_LINK_LIBRARIES (mhivesys libx264 libgcc libmingwex libcoldname libmoldname90 wsock32 vfw32 winmm  erl_interface ei libmp3lame libtheora libvorbis libvorbisenc xvidcore libogg)
  TARGET_LINK_LIBRARIES (mhivesys erl_interface ei libx264 libgcc libmingwex libcoldname libmoldname90 wsock32 vfw32 winmm libmp3lame libtheora libvorbis libvorbisenc xvidcore libogg)
#  TARGET_LINK_LIBRARIES (mhivesys erl_interface ei libgcc libmingwex libcoldname libmoldname90 wsock32 vfw32 winmm )

  target_link_libraries (mhivesys ${BZ2_LIB} io av util lang libavcodec libavformat libavutil libswscale)

ELSEIF(APPLE)
#  TARGET_LINK_LIBRARIES (erltest x264 bz2 erl_interface ei pthread)
#  TARGET_LINK_LIBRARIES (erlclient x264 bz2 erl_interface ei  pthread)
#  TARGET_LINK_LIBRARIES (mhivewriter x264 bz2 erl_interface ei  pthread)
  TARGET_LINK_LIBRARIES (mhivesys x264 z bz2 erl_interface ei  pthread mp3lame theora vorbis vorbisenc xvidcore ogg)
ELSE(WIN32)
#  TARGET_LINK_LIBRARIES (erltest x264  erl_interface ei pthread)
#  TARGET_LINK_LIBRARIES (erlclient x264  erl_interface ei  pthread)
#  TARGET_LINK_LIBRARIES (mhivewriter x264  erl_interface ei  pthread)
  TARGET_LINK_LIBRARIES (mhivesys z x264 pthread mp3lame theora vorbis vorbisenc xvidcore ogg)
ENDIF(WIN32)
#TARGET_LINK_LIBRARIES(mhivesys 
#  ${BZ2_LIB} 
#  av 
#  lang 
#  util 
#  io 
#  ${AVCODEC_LIB}
#  ${AVFORMAT_LIB}
#  ${AVUTIL_LIB}
#  ${SWSCALE_LIB}
#  ${ERL_INTERFACE_LIB}
#  ${ERL_EI_LIB}
#)

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

#INSTALL(TARGETS mhivesys
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
#    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})

#execute_process(COMMAND ${ERLANG_DIR}/bin/erl -args_file compile.args
#                  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl/
#                  OUTPUT_FILE ${PROJECT_SOURCE_DIR}/src/erl/compile.log 
#                  ERROR_FILE ${PROJECT_SOURCE_DIR}/src/erl/compile.log
#                  )

#file(GLOB ERLANG_SRC_FILES ${PROJECT_SOURCE_DIR}/src/erl/*.erl)
add_custom_target(erlang 
  ${ERLANG_DIR}/bin/erl -noshell -s devel build -s init stop
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl
)


#add_custom_target(tsystem ${ERLANG_DIR}/bin/erl -args_file system.args

add_custom_target(tsystem 
  ${ERLANG_DIR}/bin/erl -noshell -pa ebin -s make all load -s target_system create mhive -s target_system install mhive ./target -s init stop
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl
)

add_custom_target(release
  ${ERLANG_DIR}/bin/erl -noshell  -s devel clean -s devel build -s devel release -s init stop
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl
)

add_custom_target(update 
  ${ERLANG_DIR}/bin/erl -noshell -s devel clean -s devel build -s devel release  -s devel update -s init stop
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl
)

add_custom_target(clean
  ${ERLANG_DIR}/bin/erl -s devel clean -s init stop
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl
)

add_custom_target(all
  ${ERLANG_DIR}/bin/erl -noshell -s devel build -s init stop
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl
)

#INSTALL(CODE "
#${ERLANG_DIR}/bin/erl -args_file build.args
#WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl"
#)

#configure_file(mhive.app ${PROJECT_SOURCE_DIR}/src/erl/ebin/mhive.app COPYONLY)


#INSTALL(TARGETS mhivesys
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
#    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})



INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl/releases/${MHIVE_VERSION}/tmp/
  DESTINATION .
  USE_SOURCE_PERMISSIONS)

#INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl/releases/${MHIVE_VERSION}/tmp/lib/
#  DESTINATION lib
#  USE_SOURCE_PERMISSIONS)
#INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl/releases/${MHIVE_VERSION}/tmp/bin/
#  DESTINATION bin
#  USE_SOURCE_PERMISSIONS)
#INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl/releases/${MHIVE_VERSION}/tmp/data/
#  DESTINATION data
#  USE_SOURCE_PERMISSIONS)
#INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl/releases/${MHIVE_VERSION}/tmp/erts-5.7/
#  DESTINATION erts-5.7
#  USE_SOURCE_PERMISSIONS)
#INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl/releases/${MHIVE_VERSION}/tmp/logs
#  DESTINATION .
#  USE_SOURCE_PERMISSIONS)
#INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/src/erl/releases/${MHIVE_VERSION}/tmp/releases/
#  DESTINATION releases
#  USE_SOURCE_PERMISSIONS)
#INSTALL(FILES ${PROJECT_SOURCE_DIR}/src/erl//releases/${MHIVE_VERSION}/tmp/.hosts.erlang
#  DESTINATION . 
#  )
#set(CPACK_INSTALL_COMMANDS NULL)
#INSTALL(SCRIPT CMakeSetup.txt)

