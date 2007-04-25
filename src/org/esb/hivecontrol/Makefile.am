MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

#noinst_HEADERS=*.h 
noinst_LIBRARIES=libhivecontrol.a
#lib_LTLIBRARIES = libhivecontrol.a
SOURCES = hivecontrol.cpp ClientData.cpp ClientHandler.cpp
libhivecontrol_a_SOURCES = $(SOURCES) 
libhivecontrol_a_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION) -version-info $(MICRO_VERSION)
# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src \
	    $(all_includes) 
