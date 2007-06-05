#/***************************************************************************
# *   Copyright (C) 2007 by Jan Hoelscher                                   *
# *   jan.hoelscher@esblab.com                                              *
# ***************************************************************************/
SOURCES=client.cpp 
bin_PROGRAMS = client
client_SOURCES = $(SOURCES)
client_LDFLAGS = $(all_libraries) 
#client_LDADD   = $(top_srcdir)/src/org/esb/libesb.la
client_LDADD   = ../org/esb/libesb.la $(all_libraries) 


INCLUDES     = -I$(top_srcdir)/src \
	        $(all_includes) 
#noinst_HEADERS = *.h 

