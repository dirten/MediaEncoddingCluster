#/***************************************************************************
# *   Copyright (C) 2007 by Jan Hoelscher                                   *
# *   jan.hoelscher@esblab.com                                              *
# ***************************************************************************/
SUBDIRS = org hive.server hive.client hive.listener framehive


bin_PROGRAMS = framesender framereceiver


#bin_PROGRAMS = zpipe


framesender_SOURCES = bruteripper.cpp
framereceiver_SOURCES = FrameReceiver.cpp
#zpipe_SOURCES = zpipe.c

# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src/framehive \
	    $(all_includes) 

# the library search path.
framesender_LDFLAGS = $(all_libraries) 
#$(top_srcdir)/src/config/libconfig.la 
#zpipe_LDFLAGS = $(all_libraries) 
#noinst_HEADERS = *.h 

framesender_LDADD=./org/esb/libesb.la ./framehive/libframehive.la 
framereceiver_LDADD=./org/esb/libesb.la 

