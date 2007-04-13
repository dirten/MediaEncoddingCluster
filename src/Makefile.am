#/***************************************************************************
# *   Copyright (C) 2007 by Jan Hoelscher                                   *
# *   jan.hoelscher@esblab.com                                              *
# ***************************************************************************/
SUBDIRS = config
bin_PROGRAMS = framesender framereceiver zpipe
#bin_PROGRAMS = zpipe
framesender_SOURCES = bruteripper.cpp FrameContainer.cpp FrameHive.cpp FrameCompressor.cpp minilzo.c
framereceiver_SOURCES = FrameReceiver.cpp
zpipe_SOURCES = zpipe.c

# set the include path found by configure
INCLUDES= -I$(top_srcdir)/src/config $(all_includes) 

# the library search path.
framesender_LDFLAGS = $(all_libraries) $(top_srcdir)/src/config/libconfig.la 
zpipe_LDFLAGS = $(all_libraries) 
noinst_HEADERS=*.h

frameserver_LDADD=$(top_srcdir)/src/config/libconfig.la 

