MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

#noinst_HEADERS=*.h 

lib_LTLIBRARIES = libsql.la
SOURCES = Connection.cpp Statement.cpp PreparedStatement.cpp ResultSet.cpp SqlException.cpp
# set the include path found by configure
INCLUDES=-I$(top_srcdir)/src
libsql_la_SOURCES = $(SOURCES)
libsql_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
libav_la_LIBADD = ../lang/liblang.la ../io/libio.la

EXTRA_DIST=*.h *.hpp
