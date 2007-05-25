MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

#noinst_HEADERS=*.h 

lib_LTLIBRARIES = liblang.la
SOURCES = Thread.cpp Exception.cpp NoSuchElementException.cpp
# set the include path found by configure
INCLUDES=-I$(top_srcdir)/src
liblang_la_SOURCES = $(SOURCES)
liblang_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION) -version-info 0:$(MICRO_VERSION):0
EXTRA_DIST=*.h
