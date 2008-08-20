MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

#noinst_HEADERS=*.h 

lib_LTLIBRARIES = libsql.la
SOURCES = Column.cpp Connection.cpp Statement.cpp PreparedStatement.cpp ResultSet.cpp SqlException.cpp
# set the include path found by configure
INCLUDES=-I$(top_srcdir)/src
libsql_la_SOURCES = $(SOURCES)
libsql_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
libsql_la_LIBADD = ../util/libutil.la 

EXTRA_DIST=*.h 
