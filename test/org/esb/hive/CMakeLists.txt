include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)


add_executable (TestFileImporter TestFileImporter.cpp)
add_executable (TestDatabaseCreation TestDatabaseCreation.cpp)




MESSAGE("MYSQL_DRIVER" ${MYSQL_LIB})

target_link_libraries (TestFileImporter esb sql ${MYSQL_LIB} hive io av util lang avformat avcodec avutil swscale ${BOOST_SERIAL_LIB_MT})
target_link_libraries (TestDatabaseCreation esb sql ${MYSQL_LIB} hive io util lang )
IF(NOT WIN32)
	target_link_libraries (TestFileImporter x264 pthread vorbis vorbisenc ogg xvidcore mp3lame theora)
ELSE(NOT WIN32)
target_link_libraries (TestDatabaseCreation ws2_32 msvcrt wt wthttp wtext)
ENDIF(NOT WIN32)

ADD_TEST(TestFileImporter ${EXECUTABLE_OUTPUT_PATH}/TestFileImporter)
ADD_TEST(TestDatabaseCreation ${EXECUTABLE_OUTPUT_PATH}/TestDatabaseCreation)


