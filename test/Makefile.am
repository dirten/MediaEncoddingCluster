# Copyright (C) 2001 Jan Hoelscher <jh@bogs.de>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
noinst_HEADERS=*.h 

bin_PROGRAMS=server client test test_dvdread test_config
test_dvdread_SOURCES=test_dvdread.cpp 

test_config_SOURCES=test_config.cpp 

client_SOURCES=socket.cpp 
server_SOURCES=server.cpp 
test_SOURCES=	test.cpp \
	        test2.cpp 

#aviclient_LDADD =-laviplay

client_LDADD	=	$(top_srcdir)/src/org/esb/config/libconfig.la \
			$(top_srcdir)/src/org/esb/socket/libsocket.la 

server_LDADD	=	$(top_srcdir)/src/org/esb/config/libconfig.la \
			$(top_srcdir)/src/org/esb/socket/libsocket.la \
			-lpthread 

test_dvdread_LDADD		=	$(top_srcdir)/src/config/libconfig.la

test_config_LDADD		=	$(top_srcdir)/src/org/esb/config/libconfig.la


INCLUDES = \
        -I$(top_srcdir)/src/config \
        -I$(top_srcdir)/src/ \
        -I$(top_srcdir)/src/socket
