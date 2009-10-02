
SET(SQL_VERSION_SERIES 0)
SET(SQL_VERSION_MAJOR 0)
SET(SQL_VERSION_MINOR 1)

SET(WEB_VERSION_SERIES 0)
SET(WEB_VERSION_MAJOR 0)
SET(WEB_VERSION_MINOR 1)

#ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)

include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (D:/google-breakpad/src/)
include_directories (${MEC_SOURCE_DIR}/src/org/esb/av)
include_directories (${FFMPEG_INCLUDE_DIR})

LINK_DIRECTORIES(${FFMPEG_LIB_DIR})

add_library (av av av/Init.cpp 
            av/FrameConverter.cpp 
            av/FrameOutputStream.cpp 
            av/FormatInputStream.cpp 
            av/FormatBaseStream.cpp 
            av/FormatOutputStream.cpp 
            av/PacketInputStream.cpp 
            av/PacketOutputStream.cpp 
            av/Codec.cpp 
            av/Decoder.cpp 
            av/Encoder.cpp 
            av/Packet.cpp 
            av/Frame.cpp 
            av/PPSWriter.cpp
            av/AVInputStream.cpp
            av/FormatStreamFactory.cpp
            av/StreamInfo.cpp

)
TARGET_LINK_LIBRARIES(av
  ${FFMPEG_AVFORMAT_LIBRARY}
  ${FFMPEG_AVCODEC_LIBRARY}
  ${FFMPEG_AVUTIL_LIBRARY}
  ${FFMPEG_SWSCALE_LIBRARY}
  ${BZ2_LIB}
)
IF(NOT WIN32)
	target_link_libraries (av x264 pthread ssl vorbis vorbisenc ogg xvidcore mp3lame theora dl z )
ENDIF(NOT WIN32)

add_subdirectory (lang)
#TARGET_LINK_LIBRARIES(lang
#  ${BOOST_THREAD_LIB_MT}
#)
include_directories (${MEC_SOURCE_DIR}/src/org/esb/util)
add_library (util 
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
#  util/Queue.cpp
)

TARGET_LINK_LIBRARIES(util ${BOOST_DT_LIB_MT} ${BOOST_SYSTEM_LIB_MT} ${BOOST_THREAD_LIB_MT})
IF(NOT WIN32)
  TARGET_LINK_LIBRARIES(util pthread)
ENDIF(NOT WIN32)

include_directories (${MEC_SOURCE_DIR}/src/org/esb/signal)
add_library (signal
  signal/Messenger.cpp
  signal/Message.cpp
)

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
add_library (net net/SocketException.cpp 
#      net/ServerSocket.cpp 
#      net/Socket.cpp 
      net/TcpServerSocket.cpp 
      net/TcpSocket.cpp 
#      net/SocketInputStream.cpp 
#      net/SocketOutputStream.cpp
      net/TcpSocketInputStream.cpp 
      net/TcpSocketOutputStream.cpp
)
TARGET_LINK_LIBRARIES(net util)
include_directories (${MEC_SOURCE_DIR}/src/org/esb/sql)
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)
include_directories (${MYSQL_INC_DIR})
include_directories (${MYSQL_INCLUDE_DIR})
add_library (sql sql/Column.cpp 
      sql/Connection.cpp 
      sql/Statement.cpp 
      sql/PreparedStatement.cpp 
      sql/ResultSet.cpp 
      sql/ResultSetMetaData.cpp 
      sql/SqlException.cpp
      sql/ConnectionManager.cpp
      sql/Row.cpp
)
TARGET_LINK_LIBRARIES(sql ${MYSQL_LIBRARY})
IF(UNIX AND NOT APPLE)
  TARGET_LINK_LIBRARIES(sql crypt)
ENDIF(UNIX AND NOT APPLE)
#SET_TARGET_PROPERTIES(sql
#PROPERTIES
#  VERSION ${SQL_VERSION_SERIES}.${SQL_VERSION_MAJOR}.${SQL_VERSION_MINOR}
#  DEBUG_POSTFIX "d"
#)

include_directories (${MEC_SOURCE_DIR}/src/org/esb/hive)
add_library (hive 
    hive/Command.cpp 
	hive/ProtocolServer.cpp 
	hive/HiveControl.cpp 
	hive/HiveListener.cpp 
	hive/CommandInputStream.cpp 
	hive/CommandOutputStream.cpp 
	hive/PacketCollector.cpp 
	hive/CodecFactory.cpp 
	hive/job/Job.cpp 
	hive/job/JobDetail.cpp 
	hive/job/JobHandler.cpp 
	hive/job/JobWatcher.cpp 
	hive/job/ProcessUnitWatcher.cpp 
#	hive/job/ClientHandler.cpp 
	hive/job/ProcessUnit.cpp 
	hive/DirectoryScanner.cpp 
	hive/DatabaseUtil.cpp 
	hive/FileImporter.cpp 
	hive/JobUtil.cpp 
	hive/HiveClient.cpp 
	hive/Setup.cpp 
	hive/FileExporter.cpp
	hive/ExportScanner.cpp
#	hive/FormatStreamFactory.cpp
	
)
TARGET_LINK_LIBRARIES(hive av)

include_directories (${MEC_SOURCE_DIR}/src/org/esb/web)
include_directories (${WT_DIR})
include_directories (${WT_DIR}/include)
include_directories (${WT_DIR}/src)
add_library (web
    web/WebServer.cpp
    web/MainMenu.cpp
    web/WebApp.cpp
    web/WebApp2.cpp
    web/ProfilesForm.cpp
    web/WatchfolderForm.cpp
    web/DirectoryChooser.cpp
    web/SqlTableModel.cpp 
    web/SqlTable.cpp
    web/FileTreeTable.cpp
    web/FileTreeTableNode.cpp
    web/DataTreeTable.cpp
    web/DataTreeTableNode.cpp
    web/VersionNode.cpp
    web/FileNode.cpp
    web/SqlUtil.cpp
    web/Setup.cpp
    web/wtk/Div.cpp
    web/wtk/ContentBox.cpp
)
TARGET_LINK_LIBRARIES(web 
  wt
  wtext
  wthttp
  ${BOOST_SIGNALS_LIB_MT}
  ${BOOST_REGEX_LIB_MT}
)
IF(UNIX AND NOT APPLE)
  TARGET_LINK_LIBRARIES(web crypto)
ENDIF(UNIX AND NOT APPLE)

IF(WIN32)
  target_link_libraries (web
    winmm
  )
ENDIF(WIN32)
#SET_TARGET_PROPERTIES(web
#PROPERTIES
#  VERSION ${WEB_VERSION_SERIES}.${WEB_VERSION_MAJOR}.${WEB_VERSION_MINOR}
#  DEBUG_POSTFIX "d"
#)

#TARGET_LINK_LIBRARIES(av z avcodec avformat avutil  swscale x264 pthread mp3lame theora vorbis vorbisenc xvidcore ogg)

include_directories (${MEC_SOURCE_DIR}/src/org/esb/config)
add_library (esb config/config.cpp)
TARGET_LINK_LIBRARIES(esb util)
