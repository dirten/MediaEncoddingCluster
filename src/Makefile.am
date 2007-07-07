#/***************************************************************************
# *   Copyright (C) 2007 by Jan Hoelscher                                   *
# *   jan.hoelscher@esblab.com                                              *
# ***************************************************************************/
SUBDIRS = org hive.client
#hive.server hive.client hive.listener 
#framehive


bin_PROGRAMS = framesender framereceiver hivelsnr


#bin_PROGRAMS = zpipe


hivelsnr_SOURCES = Listener.cpp
framesender_SOURCES = bruteripper.cpp
framereceiver_SOURCES = FrameReceiver.cpp
#zpipe_SOURCES = zpipe.c

# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src
	    $(all_includes) 

# the library search path.
framesender_LDFLAGS = $(all_libraries) 
#$(top_srcdir)/src/config/libconfig.la 
#zpipe_LDFLAGS = $(all_libraries) 
#noinst_HEADERS = *.h 

hivelsnr_LDADD=./org/esb/libesb.la ./org/esb/io/libio.la ./org/esb/av/libav.la


framesender_LDADD=./org/esb/libesb.la 
framereceiver_LDADD=./org/esb/libesb.la 
EXTRA_DIST=introspec.h 
