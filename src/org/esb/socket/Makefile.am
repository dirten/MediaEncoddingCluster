MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

#noinst_HEADERS=*.h 

lib_LTLIBRARIES = libsocket.la
libsocket_la_SOURCES =	socket.cpp 

libsocket_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION) -version-info 0:$(MICRO_VERSION):0

#libsocket_la_LIBADD	=	$(top_srcdir)/src/org/esb/config/libconfig.la

INCLUDES 		= 	-I$(top_srcdir)/src

