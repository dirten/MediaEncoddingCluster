#/***************************************************************************
# *   Copyright (C) 2007 by Jan Hoelscher                                   *
# *   jan.hoelscher@esblab.com                                              *
# ***************************************************************************/
SUBDIRS = org
#hive.server hive.client hive.listener 
#framehive


bin_PROGRAMS = hive
hive_SOURCES = Hive.cpp

# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src
	    $(all_includes) 

# the library search path.
hive_LDADD= ./org/esb/lang/liblang.la ./org/esb/libesb.la ./org/esb/io/libio.la ./org/esb/av/libav.la ./org/esb/net/libnet.la ./org/esb/hive/libhive.la 
EXTRA_DIST=introspec.h import.cpp export.cpp job.cpp Environment.cpp CreateDatabase.cpp
