#/***************************************************************************
# *   Copyright (C) 2007 by Jan Hoelscher                                   *
# *   jan.hoelscher@esblab.com                                              *
# ***************************************************************************/
SUBDIRS = org
#hive.server hive.client hive.listener 
#framehive


bin_PROGRAMS = hivelsnr import job bla export


#bin_PROGRAMS = zpipe


#import_SOURCES = Importer.cpp
job_SOURCES = job.cpp
bla_SOURCES = bla.cpp
hivelsnr_SOURCES = Listener.cpp
import_SOURCES = import2.cpp 
export_SOURCES = export.cpp 
framesender_SOURCES = bruteripper.cpp
framereceiver_SOURCES = FrameReceiver.cpp
#zpipe_SOURCES = zpipe.c

# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src
	    $(all_includes) 

# the library search path.
framesender_LDFLAGS = $(all_libraries) 
import_LDFLAGS = $(all_libraries) 
export_LDFLAGS = $(all_libraries) 
job_LDFLAGS = $(all_libraries) 
#$(top_srcdir)/src/config/libconfig.la 
#zpipe_LDFLAGS = $(all_libraries) 
#noinst_HEADERS = *.h 

hivelsnr_LDADD= ./org/esb/lang/liblang.la ./org/esb/libesb.la ./org/esb/io/libio.la ./org/esb/av/libav.la ./org/esb/net/libnet.la ./org/esb/hive/libhive.la 
import_LDADD= ./org/esb/lang/liblang.la ./org/esb/libesb.la ./org/esb/io/libio.la ./org/esb/av/libav.la ./org/esb/net/libnet.la ./org/esb/hive/libhive.la 
export_LDADD= ./org/esb/lang/liblang.la ./org/esb/libesb.la ./org/esb/io/libio.la ./org/esb/av/libav.la ./org/esb/net/libnet.la ./org/esb/hive/libhive.la 
job_LDADD= ./org/esb/lang/liblang.la ./org/esb/libesb.la ./org/esb/io/libio.la ./org/esb/av/libav.la ./org/esb/net/libnet.la ./org/esb/hive/libhive.la 
bla_LDADD= ./org/esb/lang/liblang.la ./org/esb/libesb.la ./org/esb/io/libio.la ./org/esb/av/libav.la ./org/esb/net/libnet.la ./org/esb/hive/libhive.la 


framesender_LDADD=./org/esb/libesb.la ./org/esb/io/libio.la ./org/esb/net/libnet.la ./org/esb/av/libav.la
framereceiver_LDADD=./org/esb/libesb.la ./org/esb/io/libio.la ./org/esb/net/libnet.la ./org/esb/av/libav.la
EXTRA_DIST=introspec.h 
