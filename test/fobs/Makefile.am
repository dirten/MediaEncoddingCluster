MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1




bin_PROGRAMS = test tc

test_SOURCES=test.cpp
tc_SOURCES=tc.cpp

test_LDADD=libfobs.la
tc_LDADD=libfobs.la

lib_LTLIBRARIES = libfobs.la
SOURCES = \
	    Decoder.cpp \
            Encoder.cpp \
            Error.cpp \
            PacketBuffer.cpp \
            Transcoder.cpp 
            
            
libfobs_la_SOURCES = $(SOURCES)
libfobs_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
libfobs_la_LIBADD = -lavformat -lavutil -lavcodec -lswscale
# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src

