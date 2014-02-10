SET(SQL_VERSION_SERIES 0)
SET(SQL_VERSION_MAJOR 0)
SET(SQL_VERSION_MINOR 1)

SET(WEB_VERSION_SERIES 0)
SET(WEB_VERSION_MAJOR 0)
SET(WEB_VERSION_MINOR 1)

#SET(LIB_INSTALL_DIR lib)
SET(SHARED_CORE On)
SET(SHARED_UTIL On)
SET(SHARED_SIGNAL On)
SET(SHARED_IO On)

#ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
#ADD_DEFINITIONS(-DBOOST_USE_WINDOWS_H)
IF(WIN32)
ADD_DEFINITIONS(-DBOOST_USE_WINDOWS_H)
ENDIF(WIN32)

include_directories (${MEC_SOURCE_DIR}/build/build/include/shared)
include_directories (${MEC_SOURCE_DIR}/build/build/include/lib)

include_directories (${MEC_SOURCE_DIR})
include_directories (${LOG4CPLUS_DIR}/include)
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (D:/google-breakpad/src/)
include_directories (${MEC_SOURCE_DIR}/src/org/esb/av)
include_directories (${FFMPEG_INCLUDE_DIR})
include_directories (${LOG4CPLUS_DIR}/litesql)
include_directories (${LOG4CPLUS_DIR}/include/litesql)
include_directories (${MEC_SOURCE_DIR}target/dependency/include)
#include_directories (${MEC_SOURCE_DIR}/target/dependency/include/safmq)

#link_directories (/c/bruteripper/target/dependency/lib)

LINK_DIRECTORIES(${LOG4CPLUS_DIR}/lib)

LINK_DIRECTORIES(${LITESQL_DIR}/lib/static)
#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libogg/lib)
#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/x264/lib)
#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/xvidcore/lib)
#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/lame/lib)
#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libtheora/lib)
#LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libvorbis/lib)


add_library (db 
  db/hivedb.cpp
  db/Functions.cpp
#  db/DatabaseConfigLoader.cpp
#  db/DatabaseBootstrap.cpp
#            av/FrameConverter.cpp
#            av/FrameOutputStream.cpp
#            av/FormatInputStream.cpp
#            av/FormatBaseStream.cpp
#            av/FormatOutputStream.cpp
#            av/PacketInputStream.cpp
#            av/PacketOutputStream.cpp
#            av/Codec.cpp
#            av/Decoder.cpp
#            av/Encoder.cpp
#            av/Packet.cpp
#            av/Frame.cpp
#            av/PPSWriter.cpp
#            av/AVInputStream.cpp
#            av/FormatStreamFactory.cpp
#            av/StreamInfo.cpp
#
)
TARGET_LINK_LIBRARIES(db )
IF(NOT WIN32)
  TARGET_LINK_LIBRARIES(db litesql litesql-util litesql_sqlite)
ELSEIF(WIN32)
  IF(CMAKE_BUILD_TYPE MATCHES "debug")
    TARGET_LINK_LIBRARIES(db litesqld litesql-utild litesql_sqlited)
    ELSE()
    TARGET_LINK_LIBRARIES(db litesql litesql-util litesql_sqlite)
  ENDIF()
ENDIF(NOT WIN32)
IF(BUILD_SHARED_LIBS)
  INSTALL(TARGETS db
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
ENDIF(BUILD_SHARED_LIBS)
TARGET_LINK_LIBRARIES(db ${SQLITE_LIBRARY})


#add_library (orm orm/hivedb.cpp)
#TARGET_LINK_LIBRARIES(orm ${MYSQL_LIBRARY} )
#IF(NOT WIN32)
#  TARGET_LINK_LIBRARIES(orm litesql litesql-util litesql_mysql)
#ELSEIF(WIN32)
#  IF(CMAKE_BUILD_TYPE MATCHES "debug")
#    TARGET_LINK_LIBRARIES(orm litesqld litesql-utild litesql_mysqld)
#    ELSE()
#    TARGET_LINK_LIBRARIES(orm litesql litesql-util litesql_mysql)
#  ENDIF()
#ENDIF(NOT WIN32)
#IF(BUILD_SHARED_LIBS)
#  INSTALL(TARGETS orm
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
#    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
#    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
#ENDIF(BUILD_SHARED_LIBS)

#TARGET_LINK_LIBRARIES(av
#  ${FFMPEG_AVFORMAT_LIBRARY}
#  ${FFMPEG_AVCODEC_LIBRARY}
#  ${FFMPEG_AVUTIL_LIBRARY}
#  ${FFMPEG_SWSCALE_LIBRARY}
#  ${BZ2_LIB}
#)
#IF(NOT WIN32)
#	target_link_libraries (av x264 pthread vorbis vorbisenc ogg xvidcore mp3lame theora z )
#ENDIF(NOT WIN32)
#IF(BUILD_SHARED_LIBS)
#  INSTALL(TARGETS av
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
#    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
#    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
#ENDIF(BUILD_SHARED_LIBS)
add_subdirectory (core)
add_subdirectory (libjson)
add_subdirectory (av)
add_subdirectory (lang)
#add_subdirectory (thrift)

#add_subdirectory (mq)
add_subdirectory (grid)
#add_subdirectory (rpc)
#add_subdirectory (api)
#TARGET_LINK_LIBRARIES(lang
#  ${BOOST_THREAD_LIB_MT}
#)

IF(SHARED_UTIL)
    ADD_DEFINITIONS(-DUTIL_BUILD_SHARED)
    SET(UTIL_LINK_TYPE SHARED)
ELSE(SHARED_UTIL)
    SET(UTIL_LINK_TYPE STATIC)
ENDIF(SHARED_UTIL)
include_directories (${MEC_SOURCE_DIR}/src/org/esb/util)
add_library (util ${UTIL_LINK_TYPE}
  util/Properties.cpp 
  util/StringTokenizer.cpp 
  util/StringUtil.cpp 
  util/Date.cpp 
  util/Time.cpp 
  util/Datetime.cpp 
  util/Decimal.cpp 
  util/ScopedTimeCounter.cpp 
  util/IllegalFormatException.cpp
  util/Log.cpp
  util/Timer.cpp
  util/PropertyTree.cpp
#  util/Queue.cpp
  util/Base64.cpp
  util/UUID.cpp
)

TARGET_LINK_LIBRARIES(util lang)

IF(UNIX AND NOT APPLE)
    TARGET_LINK_LIBRARIES(util rt)
  SET(LIB_EXT -1.0.so.4)
  SET(LIB_PRE lib)
ENDIF()
IF(APPLE)
  SET(LIB_EXT -1.0.4.dylib)
  SET(LIB_PRE lib)
ENDIF()


SET_TARGET_PROPERTIES( util PROPERTIES PREFIX "libmhive-")
IF(NOT WIN32)
  #SET_TARGET_PROPERTIES( util PROPERTIES PREFIX "libmhive")
  TARGET_LINK_LIBRARIES(util pthread )
  SET(LOG4CPLUS_LIB log4cplus)
ELSEIF(WIN32)
  TARGET_LINK_LIBRARIES(util advapi32 )
  IF(CMAKE_BUILD_TYPE MATCHES "debug")
    SET(LOG4CPLUS_LIB log4cplusd)
  ELSE()
    SET(LOG4CPLUS_LIB log4cplus)
  ENDIF()
  SET(LIB_EXT .dll)
  SET(LIB_PRE)
ENDIF(NOT WIN32)
TARGET_LINK_LIBRARIES(util log4cplus )

IF(WIN32)
  TARGET_LINK_LIBRARIES(util wsock32 ws2_32 advapi32 iphlpapi)
ENDIF(WIN32)


#IF(BUILD_SHARED_LIBS)
  INSTALL(TARGETS util
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
#    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
#  INSTALL(FILES
#      ${FFMPEG_DIR}/lib/${LIB_PRE}${LOG4CPLUS_LIB}${LIB_EXT}
#    DESTINATION ${LIB_INSTALL_DIR}
#    )
#ENDIF(BUILD_SHARED_LIBS)

IF(SHARED_SIGNAL)
    ADD_DEFINITIONS(-DSIGNAL_BUILD_SHARED)
    SET(SIGNAL_LINK_TYPE SHARED)
ELSE(SHARED_SIGNAL)
    SET(SIGNAL_LINK_TYPE STATIC)
ENDIF(SHARED_SIGNAL)

include_directories (${MEC_SOURCE_DIR}/src/org/esb/signal)
add_library (signal ${SIGNAL_LINK_TYPE}
  signal/Messenger.cpp
  signal/Message.cpp
)
TARGET_LINK_LIBRARIES(signal util)
SET_TARGET_PROPERTIES( signal PROPERTIES PREFIX "libmhive-")
IF(SHARED_SIGNAL)
INSTALL(TARGETS signal
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
#    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
ENDIF(SHARED_SIGNAL)

add_subdirectory (io)

#include_directories (${MEC_SOURCE_DIR}/src/org/esb/io)
#add_library (io io/File.cpp 
#      io/FileInputStream.cpp 
#      io/FileOutputStream.cpp 
#      io/CharArrayInputStream.cpp 
#      io/CharArrayOutputStream.cpp 
#      io/BufferedOutputStream.cpp 
#      io/BufferedInputStream.cpp 
#      io/ObjectOutputStream.cpp 
#      io/ObjectInputStream.cpp
#)

#TARGET_LINK_LIBRARIES(io ${BOOST_FS_LIB_MT} ${BOOST_SYSTEM_LIB_MT})

include_directories (${MEC_SOURCE_DIR}/src/org/esb/net)
add_library (net SHARED
  net/SocketException.cpp 
#      net/ServerSocket.cpp 
#      net/Socket.cpp 
      net/UdpServerSocket.cpp
      net/TcpServerSocket.cpp
      net/TcpSocket.cpp 
#      net/SocketInputStream.cpp 
#      net/SocketOutputStream.cpp
#      net/TcpSocketInputStream.cpp
#      net/TcpSocketOutputStream.cpp
)
SET_TARGET_PROPERTIES( net PROPERTIES PREFIX "libmhive-")
TARGET_LINK_LIBRARIES(net util lang io)
  INSTALL(TARGETS net
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
    #ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)

#include_directories (${MEC_SOURCE_DIR}/src/org/esb/sql)
#include_directories (${MYSQL_DIR}/include/mysql)
#include_directories (${MYSQL_DIR}/include)
#include_directories (${MYSQL_INC_DIR})
#include_directories (${MYSQL_INCLUDE_DIR})
#add_library (sql sql/Column.cpp 
#      sql/Connection.cpp 
#      sql/Statement.cpp 
#      sql/PreparedStatement.cpp 
#      sql/ResultSet.cpp 
#      sql/ResultSetMetaData.cpp 
#      sql/SqlException.cpp
#      sql/ConnectionManager.cpp
#      sql/Row.cpp
#)
#TARGET_LINK_LIBRARIES(sql ${MYSQL_LIBRARY} util)
#IF(UNIX AND NOT APPLE)
#  TARGET_LINK_LIBRARIES(sql crypt)
#ENDIF(UNIX AND NOT APPLE)
#IF(UNIX)
#  TARGET_LINK_LIBRARIES(sql pthread util dl z)
#ENDIF(UNIX)
#SET_TARGET_PROPERTIES(sql
#PROPERTIES
#  VERSION ${SQL_VERSION_SERIES}.${SQL_VERSION_MAJOR}.${SQL_VERSION_MINOR}
#  DEBUG_POSTFIX "d"
#)
#IF(BUILD_SHARED_LIBS)
#  INSTALL(TARGETS sql
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
#    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
#    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
#ENDIF(BUILD_SHARED_LIBS)

include_directories (${MEC_SOURCE_DIR}/src/org/esb/hive)
add_library (hive SHARED
#    hive/Command.cpp 
#	hive/ProtocolServer.cpp 
#	hive/HiveControl.cpp 
#	hive/HiveListener.cpp 
#	hive/CommandInputStream.cpp 
#	hive/CommandOutputStream.cpp 
#	hive/PacketCollector.cpp
#	hive/CodecFactory.cpp 
#	hive/job/Job.cpp
#	hive/job/JobDetail.cpp
#	hive/job/JobHandler.cpp 
#	hive/job/JobWatcher.cpp 
#	hive/job/ProcessUnitWatcher.cpp
#	hive/job/PartitionManager.cpp 
	hive/job/ProcessUnit.cpp 
#	hive/DirectoryScanner.cpp 
#	hive/DatabaseUtil.cpp
	hive/DatabaseService.cpp
#	hive/FileImporter.cpp 
#	hive/JobUtil.cpp 
	hive/HiveClient.cpp
#	hive/HiveClientAudio.cpp
#	hive/Setup.cpp
#	hive/FileExporter.cpp
#	hive/ExportScanner.cpp
#	hive/FormatStreamFactory.cpp
#        hive/protocol/PartitionHandler.cpp
#  hive/job/Packetizer.cpp
#  hive/JobScanner.cpp
#  hive/NodeSender.cpp
#	hive/NodeReceiver.cpp
#  hive/NodeResolver.cpp
#  hive/job/ProcessUnitBuilder.cpp
#  hive/job/ProcessUnitController.cpp
#  hive/job/JobController.cpp
#  hive/ProcessManager.cpp
  hive/HiveException.cpp
#  hive/CodecPropertyTransformer.cpp
#  hive/PresetReader.cpp
  hive/PresetReaderJson.cpp
#  hive/PresetFileWriter.cpp
#  hive/PresetLoader.cpp
#  hive/xml/Project.cpp
#  hive/xml/JobCreator.cpp
  config/config.cpp
  hive/TestLogAppender.cpp
  hive/Environment.cpp
#  hive/EncodingClientHandler.cpp
#  hive/ClientStream.cpp
)

TARGET_LINK_LIBRARIES(hive
  db
  net
  signal
  av
  lang
  json
  util
  ${Boost_LIBRARIES}
  ${POCO_LIB_ZIP}
  ${POCO_LIB_NET}
  ${POCO_LIB_FOUNDATION}
)

INSTALL(TARGETS hive
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
#    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)

#SET_TARGET_PROPERTIES(web
#PROPERTIES
#  VERSION ${WEB_VERSION_SERIES}.${WEB_VERSION_MAJOR}.${WEB_VERSION_MINOR}
#  DEBUG_POSTFIX "d"
#)

#TARGET_LINK_LIBRARIES(av z avcodec avformat avutil  swscale x264 pthread mp3lame theora vorbis vorbisenc xvidcore ogg)

include_directories (${MEC_SOURCE_DIR}/src/org/esb/config)
#add_library (esb config/config.cpp)
#TARGET_LINK_LIBRARIES(esb util io db)
#IF(BUILD_SHARED_LIBS)
#INSTALL(TARGETS esb
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
#    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
#    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
#ENDIF(BUILD_SHARED_LIBS)

