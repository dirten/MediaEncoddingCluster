# Copyright (C) 2001 Jan Hoelscher <jh@bogs.de>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#noinst_HEADERS=*.h 

check_PROGRAMS = cpp_unit 
cpp_unit_SOURCES=cpp_unit.cpp \
                SimpleTests.cpp \
                TestSocket.cpp \
                TestCommandInputStream
cpp_unit_CXXFLAGS = $(CPPUNIT_CFLAGS)
cpp_unit_LDFLAGS = $(CPPUNIT_LIBS) ../src/org/esb/libesb.la 
#cpp_unit_LDFLAGS = -ldl


bin_PROGRAMS=server client test test_dvdread test_config test_thread console test_socket test_file test_bla test_packet
#datasize
datasize_SOURCES=datasize.cpp

test_file_SOURCES=test_file.cpp
test_packet_SOURCES=test_packet.cpp
test_bla_SOURCES=test_bla.cpp
test_dvdread_SOURCES=test_dvdread.cpp 
console_SOURCES=console.cpp
test_thread_SOURCES=test_thread.cpp 
test_socket_SOURCES=test_socket.cpp 

test_config_SOURCES=test_config.cpp 

test_server_SOURCES=test_server.cpp

client_SOURCES=socket.cpp 
server_SOURCES=server.cpp 
test_SOURCES=	test.cpp \
	        test2.cpp 

#aviclient_LDADD =-laviplay

test_file_LDADD=	../src/org/esb/libesb.la 
test_bla_LDADD=	../src/org/esb/libesb.la 
test_packet_LDADD=	../src/org/esb/libesb.la 
test_socket_LDADD=	../src/org/esb/libesb.la 
test_server_LDADD=	../src/org/esb/libesb.la 
client_LDADD	=	../src/org/esb/libesb.la 
console_LDADD   =	../src/org/esb/libesb.la 
server_LDADD	=	../src/org/esb/libesb.la \
			-lpthread 

test_thread_LDADD=	../src/org/esb/libesb.la \
			-lpthread 

test_dvdread_LDADD		=	../src/org/esb/libesb.la

test_config_LDADD		=	../src/org/esb/libesb.la


INCLUDES = \
        -I$(top_srcdir)/src/
