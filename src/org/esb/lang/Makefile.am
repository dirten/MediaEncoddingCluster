MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

#noinst_HEADERS=*.h 

lib_LTLIBRARIES = libtest.la
SOURCES = Thread.cpp
# set the include path found by configure
INCLUDES=-I$(top_srcdir)/src
libtest_la_SOURCES = $(SOURCES)
libtest_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION) -version-info 0:$(MICRO_VERSION):0
