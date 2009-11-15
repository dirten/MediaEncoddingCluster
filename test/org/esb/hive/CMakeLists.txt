include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)


add_executable (TestFileImporter TestFileImporter.cpp)
add_executable (TestProcessUnit TestProcessUnit.cpp)
add_executable (TestDatabaseCreation TestDatabaseCreation.cpp)
#add_executable (TestClientHandler TestClientHandler.cpp)
add_executable (TestHiveListener TestHiveListener.cpp)
add_executable (TestAudioAC3toMP2 TestAudioAC3toMP2.cpp)
add_executable (DeleteDatabase DeleteDatabase.cpp)
#add_executable (TestProcessUnit TestProcessUnit.cpp)
add_executable (TestNewProcessUnit TestNewProcessUnit.cpp)
add_executable (TestPacketizer TestPacketizer.cpp)





target_link_libraries (TestFileImporter esb sql hive net signal io av util lang )

target_link_libraries (TestProcessUnit esb sql hive io av util net signal  lang)

target_link_libraries (TestDatabaseCreation esb sql hive signal util io lang )
target_link_libraries (DeleteDatabase esb sql util hive signal)
target_link_libraries (TestProcessUnit esb hive signal av io util lang )
target_link_libraries (TestNewProcessUnit esb hive sql net signal av io util lang )
target_link_libraries (TestPacketizer esb hive av sql net signal io util lang )

#target_link_libraries (TestClientHandler   hive esb sql av io util lang )
target_link_libraries (TestHiveListener  hive signal esb sql av io util lang net
  ${BOOST_SERIAL_LIB_MT}
)
target_link_libraries (TestAudioAC3toMP2  hive signal esb sql av io util lang net)

IF(WIN32)
  target_link_libraries (TestDatabaseCreation ws2_32 msvcrt )
ELSE(WIN32)
	target_link_libraries (TestFileImporter x264 pthread vorbis vorbisenc ogg xvidcore mp3lame theora dl z )
	target_link_libraries (TestDatabaseCreation pthread dl z)
	target_link_libraries (DeleteDatabase pthread dl z )
  IF(NOT APPLE)
    target_link_libraries (DeleteDatabase crypt)
  ENDIF(NOT APPLE)
 # target_link_libraries(TestClientHandler x264 pthread vorbis vorbisenc ogg xvidcore mp3lame theora dl)
  target_link_libraries(TestHiveListener x264 pthread vorbis vorbisenc ogg xvidcore mp3lame theora dl)
  target_link_libraries(TestAudioAC3toMP2 x264 pthread vorbis vorbisenc ogg xvidcore mp3lame theora dl)
  target_link_libraries(TestProcessUnit x264 pthread vorbis vorbisenc ogg xvidcore mp3lame theora dl)

ENDIF(WIN32)

ADD_TEST(TestDatabaseCreation ${EXECUTABLE_OUTPUT_PATH}/TestDatabaseCreation)
ADD_TEST(TestFileImporter ${EXECUTABLE_OUTPUT_PATH}/TestFileImporter)
ADD_TEST(TestProcessUnit ${EXECUTABLE_OUTPUT_PATH}/TestFileImporter)
#ADD_TEST(TestClientHandler ${EXECUTABLE_OUTPUT_PATH}/TestClientHandler)
ADD_TEST(TestHiveListener ${EXECUTABLE_OUTPUT_PATH}/TestHiveListener)
ADD_TEST(TestAudioAC3toMP2 ${EXECUTABLE_OUTPUT_PATH}/TestAudioAC3toMP2)
ADD_TEST(DeleteDatabase ${EXECUTABLE_OUTPUT_PATH}/DeleteDatabase)
ADD_TEST(TestProcessUnit ${EXECUTABLE_OUTPUT_PATH}/TestProcessUnit )
ADD_TEST(TestNewProcessUnit ${EXECUTABLE_OUTPUT_PATH}/TestNewProcessUnit )
ADD_TEST(TestPacketizer ${EXECUTABLE_OUTPUT_PATH}/TestNewProcessUnit )

