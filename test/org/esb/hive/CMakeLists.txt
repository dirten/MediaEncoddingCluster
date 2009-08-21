include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)

LINK_DIRECTORIES(${MYSQL_DIR}/lib/mysql)
LINK_DIRECTORIES(${MYSQL_DIR}/Embedded/DLL/debug)
LINK_DIRECTORIES(${MYSQL_DIR}/lib/debug)

add_executable (TestFileImporter TestFileImporter.cpp)
add_executable (TestDatabaseCreation TestDatabaseCreation.cpp)



IF(USE_EMBEDDED_MYSQL)
  IF(WIN32)
    SET(MYSQL_LIB libmysqld)
#    SET(MYSQL_LIB mysqlserver)
  ELSE(WIN32)
    SET(MYSQL_LIB mysqld)
  ENDIF(WIN32)
ELSE(USE_EMBEDDED_MYSQL)
  IF(WIN32)
    SET(MYSQL_LIB libmysql)
  ELSE(WIN32)
    SET(MYSQL_LIB mysqlclient)
  ENDIF(WIN32)
ENDIF(USE_EMBEDDED_MYSQL)

MESSAGE("MYSQL_DRIVER" ${MYSQL_LIB})

target_link_libraries (TestFileImporter esb sql ${MYSQL_LIB} hive io av util lang avformat avcodec avutil swscale ${BOOST_SERIAL_LIB_MT})
target_link_libraries (TestDatabaseCreation esb sql ${MYSQL_LIB} hive io util lang )
IF(NOT WIN32)
	target_link_libraries (TestFileImporter x264 pthread vorbis vorbisenc ogg xvidcore mp3lame theora)
ENDIF(NOT WIN32)

ADD_TEST(TestFileImporter ${EXECUTABLE_OUTPUT_PATH}/TestFileImporter)
ADD_TEST(TestFileImporter ${EXECUTABLE_OUTPUT_PATH}/TestDatabaseCreation)


