#/***************************************************************************
# *   Copyright (C) 2007 by Jan Hoelscher                                   *
# *   jan.hoelscher@esblab.com                                              *
# ***************************************************************************/
SUBDIRS = lang util io net av hive

MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

#noinst_HEADERS=*.h 

lib_LTLIBRARIES = libesb.la
#libesb_la_LIBADD=hivecontrol/libhivecontrol.a
SOURCES = 	config/config.cpp 
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
libesb_la_SOURCES = $(SOURCES)
libesb_la_LIBADD = io/libio.la net/libnet.la lang/liblang.la util/libutil.la av/libav.la hive/libhive.la
libesb_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
EXTRA_DIST=lang/*.h io/*.h config/*.h util/*.h net/*.h av/*.h hive/*.h
