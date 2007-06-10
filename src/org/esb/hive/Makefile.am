MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1


#noinst_HEADERS=*.h 

lib_LTLIBRARIES = libhive.la
SOURCES = \
            Command.cpp \
            ProtocolServer.cpp \
            HiveControl.cpp \
            CommandInputStream.cpp \
            CommandOutputStream.cpp
            
            
            
            
libhive_la_SOURCES = $(SOURCES)
libhive_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src
