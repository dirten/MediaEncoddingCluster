templatesdir=$(libdir)/tmpl

templates_DATA=index.tmpl
# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src
#EXTRA_DIST=job/*.h job/*.cpp rdb/*.h rdb/*.cpp protocol/*.cpp

EXTRA_ENV="PATH=$$PATH:$(TNTNET_PATH)/bin"
ECPPC=env ${EXTRA_ENV} ecppc

.ecpp.cpp:
	${ECPPC} ${ECPPFLAGS} ${ECPPFLAGS_CPP} $<
.css.cpp:
	${ECPPC} ${ECPPFLAGS} ${ECPPFLAGS_CPP} $<
.png.cpp:
	${ECPPC} ${ECPPFLAGS} ${ECPPFLAGS_CPP} -b $<


MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

#SUBDIRS = job
#noinst_HEADERS=*.h 

lib_LTLIBRARIES = libweb.la
SOURCES = mainpage.ecpp upload.ecpp file.ecpp profile.ecpp style.css \
	but4_center.png \
	but4_left.png \
	but4_right.png
            
libweb_la_SOURCES = $(SOURCES)
libweb_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
libweb_la_LIBADD=  $(top_srcdir)/src/org/esb/sql/libsql.la $(top_srcdir)/src/org/esb/util/libutil.la -ltntnet

EXTRA_DIST=*.h *.tmpl *.shtml
