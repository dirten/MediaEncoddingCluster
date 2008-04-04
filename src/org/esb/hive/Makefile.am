MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

#SUBDIRS = job
#noinst_HEADERS=*.h 

lib_LTLIBRARIES = libhive.la
SOURCES = \
            Command.cpp \
            ProtocolServer.cpp \
            HiveControl.cpp \
            HiveListener.cpp \
            CommandInputStream.cpp \
            CommandOutputStream.cpp \
	    job/Job.cpp \
	    job/JobDetail.cpp \
	    job/JobHandler.cpp \
	    job/JobWatcher.cpp \
	    job/ClientHandler.cpp \
	    job/ProcessUnit.cpp 
            
            
            
            
libhive_la_SOURCES = $(SOURCES)
libhive_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
#libhive_la_LIBADD= $(top_srcdir)/src/org/esb/sql/libsql.la
# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src
EXTRA_DIST=job/*.h job/*.cpp protocol/*.cpp
