MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

#SUBDIRS = job
#noinst_HEADERS=*.h 

lib_LTLIBRARIES = libhttpd.la
SOURCES = 	compat_unix.c \
			string.c \
			shttpd.c \
			log.c \
			auth.c \
			md5.c \
			cgi.c \
			config.c \
			io_ssi.c \
			io_file.c \
			io_socket.c \
			io_ssl.c \
			io_emb.c \
			io_dir.c \
			io_cgi.c

            
libhttpd_la_SOURCES = $(SOURCES)
libhttpd_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
libhttpd_la_LIBADD= $(top_srcdir)/src/org/esb/sql/libsql.la 
#libhttpd_la_LDADD= $(top_srcdir)/src/org/esb/web/index.tmpl 
# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src
EXTRA_DIST=*.h
