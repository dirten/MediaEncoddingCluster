include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)


add_executable (TestFileImporter TestFileImporter.cpp)
add_executable (TestProcessUnit TestProcessUnit.cpp)
add_executable (TestDatabaseCreation TestDatabaseCreation.cpp)
add_executable (TestClientHandler TestClientHandler.cpp)
add_executable (TestHiveListener TestHiveListener.cpp)
add_executable (TestAudioAC3toMP2 TestAudioAC3toMP2.cpp)
add_executable (DeleteDatabase DeleteDatabase.cpp)
#add_executable (TestProcessUnit TestProcessUnit.cpp)





target_link_libraries (TestFileImporter esb sql hive io av util lang
  ${BOOST_SERIAL_LIB_MT} ${BOOST_THREAD_LIB_MT})

target_link_libraries (TestProcessUnit esb sql ${MYSQL_LIBRARY} hive io av util lang
  ${FFMPEG_AVFORMAT_LIBRARY}
  ${FFMPEG_AVCODEC_LIBRARY}
  ${FFMPEG_AVUTIL_LIBRARY}
  ${FFMPEG_SWSCALE_LIBRARY}
  ${BOOST_SERIAL_LIB_MT} ${BOOST_THREAD_LIB_MT})

target_link_libraries (TestDatabaseCreation esb sql ${MYSQL_LIBRARY} hive io util lang ${BOOST_THREAD_LIB_MT})
target_link_libraries (DeleteDatabase esb sql ${MYSQL_LIBRARY} hive io util lang ${BOOST_THREAD_LIB_MT} )
target_link_libraries (TestProcessUnit esb hive av io util lang ${BOOST_THREAD_LIB_MT}
${FFMPEG_AVFORMAT_LIBRARY} 
  ${FFMPEG_AVCODEC_LIBRARY} 
  ${FFMPEG_AVUTIL_LIBRARY} 
  ${FFMPEG_SWSCALE_LIBRARY} 
)

target_link_libraries (TestClientHandler   hive esb sql av io util lang ${MYSQL_LIBRARY}
  ${FFMPEG_LIBS}
  ${BOOST_THREAD_LIB_MT}
  ${BZ2_LIB}
)
target_link_libraries (TestHiveListener  hive signal esb sql av io util lang net
  ${BOOST_SERIAL_LIB_MT}
)
target_link_libraries (TestAudioAC3toMP2  hive signal esb sql av io util lang net  ${MYSQL_LIB} 
  ${FFMPEG_AVFORMAT_LIBRARY} 
  ${FFMPEG_AVCODEC_LIBRARY} 
  ${FFMPEG_AVUTIL_LIBRARY} 
  ${FFMPEG_SWSCALE_LIBRARY} 
${BOOST_THREAD_LIB_MT} ${BOOST_SERIAL_LIB_MT} ${BZ2_LIB} )

IF(WIN32)
  target_link_libraries (TestDatabaseCreation ws2_32 msvcrt wt wthttp wtext)
ELSE(WIN32)
	target_link_libraries (TestFileImporter x264 pthread vorbis vorbisenc ogg xvidcore mp3lame theora dl z )
	target_link_libraries (TestDatabaseCreation pthread dl z)
	target_link_libraries (DeleteDatabase pthread dl z )
  IF(NOT APPLE)
    target_link_libraries (DeleteDatabase crypt)
  ENDIF(NOT APPLE)
  target_link_libraries(TestClientHandler x264 pthread vorbis vorbisenc ogg xvidcore mp3lame theora dl z crypt)
  target_link_libraries(TestHiveListener x264 pthread vorbis vorbisenc ogg xvidcore mp3lame theora dl z crypt)
  target_link_libraries(TestAudioAC3toMP2 x264 pthread vorbis vorbisenc ogg xvidcore mp3lame theora dl z crypt)
  target_link_libraries(TestProcessUnit x264 pthread vorbis vorbisenc ogg xvidcore mp3lame theora dl z crypt)

ENDIF(WIN32)

ADD_TEST(TestDatabaseCreation ${EXECUTABLE_OUTPUT_PATH}/TestDatabaseCreation)
ADD_TEST(TestFileImporter ${EXECUTABLE_OUTPUT_PATH}/TestFileImporter)
ADD_TEST(TestProcessUnit ${EXECUTABLE_OUTPUT_PATH}/TestFileImporter)
ADD_TEST(TestClientHandler ${EXECUTABLE_OUTPUT_PATH}/TestClientHandler)
ADD_TEST(TestHiveListener ${EXECUTABLE_OUTPUT_PATH}/TestHiveListener)
ADD_TEST(TestAudioAC3toMP2 ${EXECUTABLE_OUTPUT_PATH}/TestAudioAC3toMP2)
ADD_TEST(DeleteDatabase ${EXECUTABLE_OUTPUT_PATH}/DeleteDatabase)
ADD_TEST(TestProcessUnit ${EXECUTABLE_OUTPUT_PATH}/TestProcessUnit )

