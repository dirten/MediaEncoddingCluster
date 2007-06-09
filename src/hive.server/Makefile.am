MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

#noinst_HEADERS=*.h 

lib_LTLIBRARIES = libhive.la
SOURCES = 	HiveControl.cpp \
		hive.job.control.cpp

libhive_la_SOURCES = $(SOURCES)
libhive_la_LIBADD   = ../org/esb/libesb.la $(all_libraries) 
#framereceiver_LDADD=./org/esb/libesb.la
#libhivecontrol_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION) -version-info 0:$(MICRO_VERSION):0
libhive_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src/
EXTRA_DIST=*.h *.cpp
