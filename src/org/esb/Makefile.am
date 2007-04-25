#/***************************************************************************
# *   Copyright (C) 2007 by Jan Hoelscher                                   *
# *   jan.hoelscher@esblab.com                                              *
# ***************************************************************************/
SUBDIRS = hivecontrol

MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

#noinst_HEADERS=*.h 

lib_LTLIBRARIES = libesb.la
#libesb_la_LIBADD=hivecontrol/libhivecontrol.a
SOURCES = 	lang/Thread.cpp \
		lang/Exception.cpp \
		socket/socket.cpp \
		config/config.cpp \
		util/SimpleProperties.cpp \
		util/StringTokenizer.cpp \
		net/ServerSocket.cpp \
		net/Socket.cpp \
		hivecontrol/hivecontrol.cpp \
		hivecontrol/ClientData.cpp \
		hivecontrol/ClientHandler.cpp 

# set the include path found by configure
INCLUDES=-I$(top_srcdir)/src
libesb_la_SOURCES = $(SOURCES)
libesb_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
EXTRA_DIST=lang/*.h config/*.h socket/*.h util/*.h hivecontrol/*.h net/*.h
