MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1
CFLAGS=

#noinst_HEADERS=*.h 

lib_LTLIBRARIES = libutil.la
SOURCES = Properties.cpp StringTokenizer.cpp StringUtil.cpp Date.cpp Time.cpp Datetime.cpp Decimal.cpp IllegalFormatException.cpp
libutil_la_SOURCES = $(SOURCES)
libutil_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
#libutil_LDADD=$(top_srcdir)/src/org/esb/lang/liblang.la  
INCLUDES=-I$(top_srcdir)/src
#EXTRA_DIST=lang/*.h io/*.h config/*.h util/*.h hivecontrol/*.h net/*.h
