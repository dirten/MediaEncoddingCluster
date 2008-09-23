

ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)

include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})

include_directories (${MEC_SOURCE_DIR}/src/org/esb/av)
include_directories (${FFMPEG_DIR} ${FFMPEG_INC_DIR})
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
)

include_directories (${MEC_SOURCE_DIR}/src/org/esb/lang)
add_library (lang lang/Byte.cpp 
      lang/String.cpp 
      lang/Thread.cpp 
      lang/Exception.cpp 
      lang/NoSuchElementException.cpp 
      lang/NullPointerException.cpp
)

include_directories (${MEC_SOURCE_DIR}/src/org/esb/util)
add_library (util util/Properties.cpp 
      util/StringTokenizer.cpp 
      util/StringUtil.cpp 
      util/Date.cpp 
      util/Time.cpp 
      util/Datetime.cpp 
      util/Decimal.cpp 
      util/IllegalFormatException.cpp
)

include_directories (${MEC_SOURCE_DIR}/src/org/esb/signal)
add_library (signal signal/Messenger.cpp signal/Message.cpp)

include_directories (${MEC_SOURCE_DIR}/src/org/esb/io)
add_library (io io/File.cpp 
      io/FileInputStream.cpp 
      io/FileOutputStream.cpp 
      io/CharArrayInputStream.cpp 
      io/CharArrayOutputStream.cpp 
      io/BufferedOutputStream.cpp 
      io/BufferedInputStream.cpp 
      io/ObjectOutputStream.cpp 
      io/ObjectInputStream.cpp
)

include_directories (${MEC_SOURCE_DIR}/src/org/esb/net)
add_library (net net/SocketException.cpp 
      net/ServerSocket.cpp 
      net/Socket.cpp 
      net/TcpServerSocket.cpp 
      net/TcpSocket.cpp 
      net/SocketInputStream.cpp 
      net/SocketOutputStream.cpp
#      net/TcpSocketInputStream.cpp 
#      net/TcpSocketOutputStream.cpp
)

include_directories (${MEC_SOURCE_DIR}/src/org/esb/sql)
include_directories (${MYSQL_INC_DIR})
add_library (sql sql/Column.cpp 
      sql/Connection.cpp 
      sql/Statement.cpp 
      sql/PreparedStatement.cpp 
      sql/ResultSet.cpp 
      sql/ResultSetMetaData.cpp 
      sql/SqlException.cpp
)

include_directories (${MEC_SOURCE_DIR}/src/org/esb/hive)
add_library (hive hive/Command.cpp 
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
	hive/job/ClientHandler.cpp 
	hive/job/ProcessUnit.cpp 
	hive/DirectoryScanner.cpp 
	hive/DatabaseUtil.cpp 
	hive/FileImporter.cpp 
	hive/JobUtil.cpp 
)

include_directories (${MEC_SOURCE_DIR}/src/org/esb/web)
include_directories (${WT_DIR})
include_directories (${WT_DIR}/src)
add_library (web web/WebServer.cpp 
    web/WebApp.cpp 
    web/SqlTableModel.cpp 
    web/SqlTable.cpp
)
include_directories (${MEC_SOURCE_DIR}/src/org/esb/config)
add_library (esb config/config.cpp)
