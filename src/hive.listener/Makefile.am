#/***************************************************************************
# *   Copyright (C) 2007 by Jan Hoelscher                                   *
# *   jan.hoelscher@esblab.com                                              *
# ***************************************************************************/
SOURCES=listener.cpp \
	proto_dispatcher.cpp 

bin_PROGRAMS = lsnr
lsnr_SOURCES = $(SOURCES)
lsnr_LDFLAGS = $(all_libraries) 
lsnr_LDADD   = ../org/esb/libesb.la ../hive.server/libhive.la $(all_libraries) 

INCLUDES     = -I$(top_srcdir)/src \
	        $(all_includes) 
#noinst_HEADERS = *.h 

