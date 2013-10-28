ADD_DEFINITIONS(-DMEC_SOURCE_DIR="${MEC_SOURCE_DIR}")
#ENABLE_TESTING()
#ADD_TEST(FIRSTTEST ${EXECUTABLE_OUTPUT_PATH}/info "/media/out/ChocolateFactory.mp2")


include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${FFMPEG_INCLUDE_DIR})
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${LOG4CPLUS_DIR}/include)
include_directories (${SAFMQ_DIR}/include/lib)
include_directories (${SAFMQ_DIR}/include/shared)

#include_directories (${MEC_SOURCE_DIR}/test/fobs/dist/include)
#INCLUDE_DIRECTORIES (${ERLANG_INTERFACE_DIR}/include)
include_directories (${MEC_SOURCE_DIR}/target/dependency/include/litesql)


INCLUDE_DIRECTORIES (${BREAKPAD_INCLUDE_DIR})

#LINK_DIRECTORIES(${BOOST_LIB_DIRS})
#LINK_DIRECTORIES(${FFMPEG_LIB_DIR})
#LINK_DIRECTORIES(${BREAKPAD_LIB_DIR})
LINK_DIRECTORIES(${LOG4CPLUS_DIR}/lib)
LINK_DIRECTORIES(${LITESQL_DIR}/lib/static)

#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/test/fobs/dist/lib)
#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libogg/lib)
#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/x264/lib)
#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/xvidcore/lib)
#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/lame/lib)
#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libtheora/lib)
#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libvorbis/lib)

#LINK_DIRECTORIES(${MYSQL_DIR}/lib/mysql)
#LINK_DIRECTORIES(${MYSQL_DIR}/Embedded/DLL/debug)
#LINK_DIRECTORIES(${MYSQL_DIR}/lib/debug)




IF(BUILD_TESTING)
#
# In order to run tests we will need to set the approriate environment
# variable so that the test program can locate its dependent DLL's. First
# we want to know what directory our dependent DLL was installed into:
#
get_target_property(utildir util LOCATION_Debug)
get_filename_component(utildir "${utildir}" PATH)
file(TO_NATIVE_PATH "${utildir}" native_utildir)


get_target_property(iodir io LOCATION_Debug)
get_filename_component(iodir "${iodir}" PATH)
file(TO_NATIVE_PATH "${iodir}" native_iodir)

#file(TO_NATIVE_PATH "${utildir}" native_utildir)
message("UtilDir" ${utildir})
#
# Next, for our directory full of platform-dependent 3rd party DLL's
#
#file(TO_NATIVE_PATH "${MEC_SOURCE_DIR}/target/dependency/lib" native_3rdparty_lib)
#file(TO_NATIVE_PATH "${MEC_SOURCE_DIR}/target/dependency/bin" native_3rdparty_bin)

#
# Determine which environment variable controls the locating of
# DLL's and set that variable.
#
if (WIN32)
   set (LD_VARNAME "PATH")
   #string(REPLACE ";" ";" PATH "$ENV{PATH}")
   #MESSAGE("ENVPATH=" $ENV{PATH})
   #MESSAGE("PATH=" ${PATH})
   set (LD_PATH "${native_utildir};${native_iodir};${MEC_SOURCE_DIR}/target/dependency/lib;${native_3rdparty_bin};$ENV{PATH}")
	
   #
   # IMPORTANT NOTE: The set_tests_properties(), below, internally
   # stores its name/value pairs with a semicolon delimiter.
   # because of this we must protect the semicolons in the path
   #
   string(REPLACE ";" "\\;" LD_PATH "${LD_PATH}")
  
else ()
   set (LD_VARNAME "LD_LIBRARY_PATH")
   set (LD_PATH "${native_utildir}:${native_3rdparty_lib}:$ENV{LD_LIBRARY_PATH}")
endif ()

#add_test (SomeTest SomeTest)
#set_tests_properties(SomeTest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

  IF(WIN32)
    SET( ENV{PATH} "$ENV{PATH}\;${FFMPEG_DIR}/lib" )
  ELSEIF(APPLE)
    SET( ENV{DYLD_LIBRARY_PATH} "${MEC_SOURCE_DIR}/target/dependency/lib" )
  ENDIF(WIN32)
  IF(UNIX)
    #execute_process(COMMAND chmod +x ${MEC_SOURCE_DIR}/target/dependency/bin/mysqld)
  ENDIF(UNIX)
  add_subdirectory (org)
  add_subdirectory (plugins)
ENDIF(BUILD_TESTING)


FILE(GLOB ERLANG_INTERFACE_DIR ${ERLANG_DIR}/lib/erl_interface-*)
#MESSAGE("Erlang Interface Dir for test" ${ERLANG_INTERFACE_DIR})


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



#ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)


#add_executable (test test.cpp)
#IF(NOT WIN32)
#  add_executable (erltest erltest.cpp)
#  target_link_libraries (erltest avcodec avformat avutil swscale z ${BZ2_LIB} io av util lang faac faad gsm mp3lame ogg theora vorbis vorbisenc xvidcore libgcc mingwex wsock32 vfw32 pthreadGC2 Winmm a52  )
#ENDIF(NOT WIN32)

add_executable (info info.cpp)
add_executable (pu pu.cpp)
add_executable (pc PacketCount.cpp)
#add_executable (testclass TestNewClass.cpp)

add_executable (mytest test.cpp)

target_link_libraries (info
  hive
  av
  util
  signal
  io
  net
  lang
  )
target_link_libraries (pu
  hive
  av
  util
  io
  net
  signal
  lang
  )
target_link_libraries (pc
  hive
  av
  util
  io
  net
  signal
  lang
  )


IF(WIN32)
#  TARGET_LINK_LIBRARIES (info erl_interface ei libx264 wsock32 vfw32 winmm libmp3lame libtheora libvorbis libvorbisenc xvidcore libogg)
#  TARGET_LINK_LIBRARIES (info erl_interface ei  libgcc libmingwex libcoldname libmoldname90 wsock32 vfw32 winmm )
#target_link_libraries (info  )

#target_link_libraries (info ${BZ2_LIB} io av util lang avcodec avformat avutil swscale   )
#target_link_libraries (test avcodec avformat avutil swscale ${BZ2_LIB} io av util lang libx264 wsock32 vfw32 winmm libmp3lame libtheora libvorbis libvorbisenc xvidcore libogg )
ELSEIF(APPLE)
  target_link_libraries (info z pthread z )
target_link_libraries (info bz2 )
#target_link_libraries (test bz2 )
#target_link_libraries (erltest bz2 )
ELSE(WIN32)
  target_link_libraries (info z dl crypt pthread z )
ENDIF(WIN32)

target_link_libraries (info
${Boost_LIBRARIES}
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

#MESSAGE("BOOST FileSystem Library" ${BOOST_FS_LIB_MT})


IF(NOT WIN32)

#target_link_libraries (info ${BZ2_LIB} erl_interface ei io av util lang avformat avcodec avutil swscale  x264 pthread z mp3lame theora vorbis vorbisenc xvidcore ogg)
#target_link_libraries (test ${BZ2_LIB} io av util lang avformat avcodec avutil swscale  x264 pthread z mp3lame theora vorbis vorbisenc xvidcore ogg)
#target_link_libraries (erltest ${BZ2_LIB} erl_interface ei io av util lang avformat avcodec avutil swscale  x264 pthread z mp3lame theora vorbis vorbisenc xvidcore ogg)
ENDIF(NOT WIN32)

