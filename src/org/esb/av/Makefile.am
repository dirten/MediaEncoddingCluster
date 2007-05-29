MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1


#noinst_HEADERS=*.h 

lib_LTLIBRARIES = libav.la
SOURCES = \
            FrameInputStream.cpp \
            FormatInputStream.cpp \
            FormatBaseStream.cpp \
            FormatOutputStream.cpp \
            AVInputStream.cpp \
            Codec.cpp
            
            
libav_la_SOURCES = $(SOURCES)
libav_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION) -version-info $(MICRO_VERSION)
# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src
#	    $(all_includes) 

#noinst_HEADERS = *.h 
