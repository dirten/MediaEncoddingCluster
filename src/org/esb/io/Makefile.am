MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1
CFLAGS=

#noinst_HEADERS=*.h 

lib_LTLIBRARIES = libio.la
SOURCES = File.cpp FileInputStream.cpp 
libio_la_SOURCES = $(SOURCES)
libio_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION) -version-info $(MICRO_VERSION)
# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src 
#	    $(all_includes) 

#noinst_HEADERS = *.h 
