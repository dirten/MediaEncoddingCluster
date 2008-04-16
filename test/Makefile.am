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
SUBDIRS=fobs

check_PROGRAMS = cpp_unit 
cpp_unit_SOURCES=cpp_unit.cpp \
                TestFile.cpp \
                TestFormatInputStream.cpp \
                TestFileOutputStream.cpp \
            	TestOrgEsbNet.cpp \
            	TestOrgEsbSql.cpp \
                TestProtocolCommands.cpp 
#                TestSocket.cpp 
#                TestCommandInputStream.cpp \
#                TestProtocolServer.cpp \
#                TestProperties.cpp
                
                
                
cpp_unit_CXXFLAGS = $(CPPUNIT_CFLAGS)
cpp_unit_LDFLAGS = $(CPPUNIT_LIBS) ../src/org/esb/libesb.la
cpp_unit_LDADD=	../src/org/esb/libesb.la
#cpp_unit_LDFLAGS = -ldl

bin_PROGRAMS=test_mysql test_httpd test_tnt test_seek test_codec outex
#bin_PROGRAMS=server client test test_dvdread test_config test_thread console test_socket test_file test_bla test_packet
#datasize
#datasize_SOURCES=datasize.cpp

#test_file_SOURCES=test_file.cpp
test_tnt_SOURCES=test_tnt.cpp
test_seek_SOURCES=test_seek.cpp
#outex_SOURCES=output_example.c
outex_SOURCES=apiexample.c
seek_test_SOURCES=seek_test.c
test_codec_SOURCES=test_codec.cpp
#test_codec_SOURCES=test_codec.cpp
test_httpd_SOURCES=test_httpd.cpp
#test_sqlite_SOURCES=test_sqlite.cpp
test_mysql_SOURCES=test_mysql.cpp
#test_packet_SOURCES=test_packet.cpp
#test_packet_input_SOURCES=test_packet_input.cpp
#test_bla_SOURCES=test_bla.cpp
#test_audio_SOURCES=test_audio.cpp
#test_dvdread_SOURCES=test_dvdread.cpp 
#test_server_SOURCES=test_server.cpp 
#test_client_SOURCES=test_client.cpp 
#console_SOURCES=console.cpp
#test_thread_SOURCES=test_thread.cpp 
#test_socket_SOURCES=test_socket.cpp 

#test_config_SOURCES=test_config.cpp 

#test_server_SOURCES=test_server.cpp

#client_SOURCES=socket.cpp 
#server_SOURCES=server.cpp 
#test_SOURCES=	test.cpp \
#	        test2.cpp 

#aviclient_LDADD =-laviplay
#test_LDFLAGS = $(all_libraries) 
test_httpd_LDFLAGS = $(all_libraries) 
test_codec_LDFLAGS = $(all_libraries) 
#test_sqlite_LDFLAGS = $(all_libraries) 
#test_packet_LDFLAGS = $(all_libraries) 
#test_packet_input_LDFLAGS = $(all_libraries) 
#test_bla_LDFLAGS = $(all_libraries) 
test_mysql_LDFLAGS = $(all_libraries) 
#test_audio_LDFLAGS = $(all_libraries) 
#server_LDFLAGS = $(all_libraries) 

#test_file_LDADD=	../src/org/esb/libesb.la 
test_tnt_LDADD=	../src/org/esb/libesb.la 
test_codec_LDADD=	../src/org/esb/libesb.la 
test_seek_LDADD=	../src/org/esb/libesb.la 
seek_test_LDADD=	../src/org/esb/libesb.la 
#test_codec_LDADD=	../src/org/esb/libesb.la 
test_httpd_LDADD=	../src/org/esb/libesb.la ../src/org/esb/io/libio.la ../src/org/esb/av/libav.la ../src/org/esb/web/libweb.la 
#test_bla_LDADD=	../src/org/esb/libesb.la ../src/org/esb/av/libav.la
test_mysql_LDADD=	../src/org/esb/libesb.la
#test_audio_LDADD=	../src/org/esb/libesb.la ../src/org/esb/av/libav.la
#test_packet_LDADD=	../src/org/esb/libesb.la ../src/org/esb/lang/liblang.la  ../src/org/esb/av/libav.la
#test_sqlite_LDADD=	../src/org/esb/lang/liblang.la ../src/org/esb/io/libio.la ../src/org/esb/av/libav.la ../src/org/esb/sql/libsql.la
#test_packet_input_LDADD= ../src/org/esb/libesb.la ../src/org/esb/lang/liblang.la ../src/org/esb/av/libav.la
#test_socket_LDADD=	../src/org/esb/libesb.la 
#server_LDADD= ../src/org/esb/libesb.la ../src/org/esb/lang/liblang.la ../src/org/esb/av/libav.la
#test_server_LDADD=	../src/org/esb/libesb.la 
#client_LDADD	=	../src/org/esb/libesb.la 
#console_LDADD   =	../src/org/esb/libesb.la 
#test_server_LDADD	=../src/org/esb/libesb.la
#test_client_LDADD	=../src/org/esb/lang/liblang.la ../src/org/esb/io/libio.la ../src/org/esb/av/libav.la ../src/org/esb/net/libnet.la ../src/org/esb/sql/libsql.la

#test_thread_LDADD=	../src/org/esb/libesb.la \
#			-lpthread 

#test_dvdread_LDADD		=	../src/org/esb/libesb.la

#test_config_LDADD		=	../src/org/esb/libesb.la


INCLUDES = \
        -I$(top_srcdir)/src/
