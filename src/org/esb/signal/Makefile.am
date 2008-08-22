MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1
CFLAGS=


lib_LTLIBRARIES = libsignal.la
SOURCES = Messenger.cpp Message.cpp
libsignal_la_SOURCES = $(SOURCES)
libsignal_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION) 
INCLUDES=   -I$(top_srcdir)/src 
