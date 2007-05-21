#/***************************************************************************
# *   Copyright (C) 2007 by Jan Hoelscher                                   *
# *   jan.hoelscher@esblab.com                                              *
# ***************************************************************************/
SOURCES=listener.cpp \
	proto_dispatcher.cpp 

bin_PROGRAMS = lsnr
lsnr_SOURCES = $(SOURCES)
lsnr_LDFLAGS = $(all_libraries) 
lsnr_LDADD   = $(top_srcdir)/src/org/esb/libesb.la $(top_srcdir)/src/hive.server/libhive.la

INCLUDES     = -I$(top_srcdir)/src \
	        $(all_includes) 
#noinst_HEADERS = *.h 

