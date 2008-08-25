#/***************************************************************************
# *   Copyright (C) 2007 by Jan Hoelscher                                   *
# *   jan.hoelscher@esblab.com                                              *
# ***************************************************************************/
MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

lib_LTLIBRARIES = \
      liblang.la \
      libutil.la \
      libsignal.la \
      libio.la \
      libnet.la \
      libsql.la \
      libav.la \
      libesb.la \
      libhive.la \
      libweb.la







liblang_la_SOURCES = \
      lang/Byte.cpp \
      lang/String.cpp \
      lang/Thread.cpp \
      lang/Exception.cpp \
      lang/NoSuchElementException.cpp \
      lang/NullPointerException.cpp
liblang_la_CXXFLAGS =  -I$(top_srcdir)/src/org/esb/lang
liblang_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)

libutil_la_SOURCES = \
      util/Properties.cpp \
      util/StringTokenizer.cpp \
      util/StringUtil.cpp \
      util/Date.cpp \
      util/Time.cpp \
      util/Datetime.cpp \
      util/Decimal.cpp \
      util/IllegalFormatException.cpp
libutil_la_CXXFLAGS =  -I$(top_srcdir)/src/org/esb/util
libutil_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)


libsignal_la_SOURCES = signal/Messenger.cpp signal/Message.cpp
libsignal_la_CXXFLAGS =  -I$(top_srcdir)/src/org/esb/signal
libsignal_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION) 


libio_la_SOURCES = \
      io/File.cpp \
      io/FileInputStream.cpp \
      io/FileOutputStream.cpp \
      io/CharArrayInputStream.cpp \
      io/CharArrayOutputStream.cpp \
      io/BufferedOutputStream.cpp \
      io/BufferedInputStream.cpp \
      io/ObjectOutputStream.cpp \
      io/ObjectInputStream.cpp
      
libio_la_CXXFLAGS =  -I$(top_srcdir)/src/org/esb/io
libio_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION) 


libnet_la_SOURCES = \
      net/SocketException.cpp \
      net/ServerSocket.cpp \
      net/Socket.cpp \
      net/SocketInputStream.cpp \
      net/SocketOutputStream.cpp
libnet_la_CXXFLAGS =  -I$(top_srcdir)/src/org/esb/net
libnet_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION) 


libsql_la_SOURCES = \
      sql/Column.cpp \
      sql/Connection.cpp \
      sql/Statement.cpp \
      sql/PreparedStatement.cpp \
      sql/ResultSet.cpp \
      sql/SqlException.cpp
libsql_la_CXXFLAGS =  -I$(top_srcdir)/src/org/esb/sql
libsql_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)


libav_la_SOURCES = \
	    av/Init.cpp \
            av/FrameConverter.cpp \
            av/FrameOutputStream.cpp \
            av/FormatInputStream.cpp \
            av/FormatBaseStream.cpp \
            av/FormatOutputStream.cpp \
            av/PacketInputStream.cpp \
            av/PacketOutputStream.cpp \
            av/Codec.cpp \
            av/Decoder.cpp \
            av/Encoder.cpp \
            av/Packet.cpp \
            av/Frame.cpp \
	        av/PPSWriter.cpp \
	        av/AVInputStream.cpp 
#	        VideoFrame.cpp \
#	        AudioFrame.cpp
            
#           CodecInputStream.cpp \
#            CodecOutputStream.cpp 
#            FrameInputStream.cpp 
            
#libav_la_SOURCES = $(SOURCES)
libav_la_CXXFLAGS =  -I$(top_srcdir)/src/org/esb/av
libav_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
libav_la_LIBADD = -lavformat -lavutil -lavcodec -lswscale



libhive_la_SOURCES = \
	hive/Command.cpp \
	hive/ProtocolServer.cpp \
	hive/HiveControl.cpp \
	hive/HiveListener.cpp \
	hive/CommandInputStream.cpp \
	hive/CommandOutputStream.cpp \
	hive/PacketCollector.cpp \
	hive/CodecFactory.cpp \
	hive/job/Job.cpp \
	hive/job/JobDetail.cpp \
	hive/job/JobHandler.cpp \
	hive/job/JobWatcher.cpp \
	hive/job/ProcessUnitWatcher.cpp \
	hive/job/ClientHandler.cpp \
	hive/job/ProcessUnit.cpp 
          
libhive_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
libhive_la_CXXFLAGS =  -I$(top_srcdir)/src/org/esb/hive
#libhive_la_LIBADD= $(top_srcdir)/src/org/esb/sql/libsql.la $(top_srcdir)/src/org/esb/libav.la $(top_srcdir)/src/org/esb/signal/libsignal.la $(top_srcdir)/src/org/esb/net/libnet.la 


libweb_la_SOURCES = web/WebServer.cpp 
libweb_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
libweb_la_CXXFLAGS =  -I$(top_srcdir)/src/org/esb/web
#libweb_la_LIBADD=  $(top_srcdir)/src/org/esb/sql/libsql.la $(top_srcdir)/src/org/esb/util/libutil.la 


#noinst_HEADERS=*.h 

#lib_LTLIBRARIES = libesb.la
#libesb_la_LIBADD=hivecontrol/libhivecontrol.a
libesb_la_SOURCES =	config/config.cpp 
#		hivecontrol/hivecontrol.cpp \
#		hivecontrol/ClientData.cpp 
#		hivecontrol/ClientHandler.cpp 
#		io/FrameInputStream.cpp 
#		net/Socket.cpp \
#		net/SocketInputStream.cpp \
#		lang/Thread.cpp \
#		util/SimpleProperties.cpp \
#		util/StringTokenizer.cpp \
#		io/InputStream.h \
#		io/OutputStream.h \
#		util/StringUtil.cpp \
#		lang/Exception.cpp \
#		net/ServerSocket.cpp \
#		net/SocketOutputStream.cpp 


# set the include path found by configure
INCLUDES=-I$(top_srcdir)/src
#libesb_la_SOURCES = $(SOURCES)
#libesb_la_LIBADD = lang/liblang.la io/libio.la net/libnet.la util/libutil.la av/libav.la hive/libhive.la web/libweb.la signal/libsignal.la
libesb_la_CXXFLAGS =  -I$(top_srcdir)/src/org/esb/config
libesb_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
EXTRA_DIST=lang/*.h io/*.h config/*.h util/*.h net/*.h av/*.h hive/*.h
