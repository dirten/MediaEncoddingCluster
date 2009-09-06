include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)
include_directories (${WT_DIR}/include)

add_executable (TestWebServer TestWebServer.cpp)

target_link_libraries (TestWebServer web esb sql hive lang av util signal io net wthttp wtext wt  
  ${MYSQL_LIBRARY} 
  ${FFMPEG_AVFORMAT_LIBRARY} 
  ${FFMPEG_AVCODEC_LIBRARY} 
  ${FFMPEG_AVUTIL_LIBRARY} 
  ${FFMPEG_SWSCALE_LIBRARY} 
 )

IF(NOT WIN32)
	target_link_libraries (TestWebServer x264 pthread ssl vorbis vorbisenc ogg xvidcore mp3lame theora dl z crypt)
ELSE(NOT WIN32)
  target_link_libraries (TestWebServer ws2_32 msvcrt )
ENDIF(NOT WIN32)

target_link_libraries (TestWebServer
  ${BOOST_SERIAL_LIB_MT}
  ${BOOST_THREAD_LIB_MT}
  ${BOOST_SYSTEM_LIB_MT}
  ${BOOST_PO_LIB_MT}
  ${BOOST_SIGNALS_LIB_MT}
  ${BOOST_REGEX_LIB_MT}
  ${BOOST_FS_LIB_MT}
  ${BOOST_DT_LIB_MT}
)
ADD_TEST(TestWebServer ${EXECUTABLE_OUTPUT_PATH}/TestWebServer)

