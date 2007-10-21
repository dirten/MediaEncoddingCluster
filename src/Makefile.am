#/***************************************************************************
# *   Copyright (C) 2007 by Jan Hoelscher                                   *
# *   jan.hoelscher@esblab.com                                              *
# ***************************************************************************/
SUBDIRS = org hive.client
#hive.server hive.client hive.listener 
#framehive


bin_PROGRAMS = hivelsnr import


#bin_PROGRAMS = zpipe


import_SOURCES = Importer.cpp
hivelsnr_SOURCES = Listener.cpp
import_SOURCES = import.cpp 
framesender_SOURCES = bruteripper.cpp
framereceiver_SOURCES = FrameReceiver.cpp
#zpipe_SOURCES = zpipe.c

# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src
	    $(all_includes) 

# the library search path.
framesender_LDFLAGS = $(all_libraries) 
import_LDFLAGS = $(all_libraries) 
#$(top_srcdir)/src/config/libconfig.la 
#zpipe_LDFLAGS = $(all_libraries) 
#noinst_HEADERS = *.h 

hivelsnr_LDADD= ./org/esb/lang/liblang.la ./org/esb/libesb.la ./org/esb/io/libio.la ./org/esb/av/libav.la ./org/esb/net/libnet.la ./org/esb/hive/libhive.la 
import_LDADD= ./org/esb/lang/liblang.la ./org/esb/libesb.la ./org/esb/io/libio.la ./org/esb/av/libav.la ./org/esb/net/libnet.la ./org/esb/hive/libhive.la 


framesender_LDADD=./org/esb/libesb.la ./org/esb/io/libio.la ./org/esb/net/libnet.la ./org/esb/av/libav.la
framereceiver_LDADD=./org/esb/libesb.la ./org/esb/io/libio.la ./org/esb/net/libnet.la ./org/esb/av/libav.la
EXTRA_DIST=introspec.h 
