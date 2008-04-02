templatesdir=$(libdir)/tmpl

templates_DATA=index.tmpl
# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src
#EXTRA_DIST=job/*.h job/*.cpp rdb/*.h rdb/*.cpp protocol/*.cpp


MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

#SUBDIRS = job
#noinst_HEADERS=*.h 

lib_LTLIBRARIES = libweb.la
SOURCES = WTK.cpp WebServer.cpp Files.cpp Jobs.cpp Stream.cpp Page.cpp Profiles.cpp
            
libweb_la_SOURCES = $(SOURCES)
libweb_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
libweb_la_LIBADD= $(top_srcdir)/src/org/esb/httpd/libhttpd.la $(top_srcdir)/src/org/esb/sql/libsql.la $(top_srcdir)/src/org/esb/util/libutil.la

EXTRA_DIST=*.h *.tmpl *.shtml
