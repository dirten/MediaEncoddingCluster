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
            CommandInputStream.cpp \
            CommandOutputStream.cpp \
	    job/Job.cpp \
	    job/JobHandler.cpp \
	    BundleIndex.cpp \
	    BundleIndexWriter.cpp \
	    rdb/HiveDbConnection.cpp
            
            
            
            
libhive_la_SOURCES = $(SOURCES)
libhive_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src
