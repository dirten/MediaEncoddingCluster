MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1
CFLAGS=

#noinst_HEADERS=*.h 

lib_LTLIBRARIES = libframehive.la
SOURCES = FrameHive.cpp FrameCompressor.cpp minilzo.c
libframehive_la_SOURCES = $(SOURCES)
libframehive_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION) -version-info 0:$(MICRO_VERSION):0
# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src \
	    $(all_includes) 

#noinst_HEADERS = *.h 
