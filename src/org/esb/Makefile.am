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
SOURCES = 	lang/Thread.cpp \
		lang/Exception.cpp \
		socket/socket.cpp \
		config/config.cpp \
		util/StringTokenizer.cpp

# set the include path found by configure
INCLUDES=-I$(top_srcdir)/src
libesb_la_SOURCES = $(SOURCES)
libesb_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION) -version-info 0:$(MICRO_VERSION):0
EXTRA_DIST=lang/*.h
