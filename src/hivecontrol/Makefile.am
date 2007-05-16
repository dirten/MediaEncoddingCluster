MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

#noinst_HEADERS=*.h 

lib_LTLIBRARIES = libhivecontrol.la
SOURCES = hivecontrol.cpp
libhivecontrol_la_SOURCES = $(SOURCES)
#libhivecontrol_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION) -version-info 0:$(MICRO_VERSION):0
libhivecontrol_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src/
