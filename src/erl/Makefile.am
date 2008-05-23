#/***************************************************************************
# *   Copyright (C) 2007 by Jan Hoelscher                                   *
# *   jan.hoelscher@esblab.com                                              *
# ***************************************************************************/
#SUBDIRS = org
#hive.server hive.client hive.listener 
#framehive


bin_PROGRAMS = erltest
erltest_SOURCES = erltest.cpp

# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src -I/usr/local/erlang/lib/erlang/lib/erl_interface-3.5.6/include -I/home/jhoelscher/bripper/MediaEncodingCluster/src/ \
		-I/home/jhoelscher/bripper/ffmpeg-install/include -I/usr/local/include/boost-1_34_1/ -I/home/jhoelscher/bripper/tnt/cxxtools/include

# the library search path.
erltest_LDADD=-L/usr/local/erlang/lib/erlang/lib/erl_interface-3.5.6/lib -lerl_interface -lei -lpthread -lstdc++ \
		/home/jhoelscher/bripper/MediaEncodingCluster/src/org/esb/av/.libs/libav.a \
		/home/jhoelscher/bripper/MediaEncodingCluster/src/org/esb/io/.libs/libio.a \
		/home/jhoelscher/bripper/tnt/cxxtools/lib/libcxxtools.a \
		/home/jhoelscher/bripper/ffmpeg-install/lib/libavformat.a \
		/home/jhoelscher/bripper/ffmpeg-install/lib/libavcodec.a \
		/home/jhoelscher/bripper/ffmpeg-install/lib/libavutil.a \
		/home/jhoelscher/bripper/x264/lib/libx264.a \
		/home/jhoelscher/bripper/ffmpeg-install/lib/libswscale.a -lz
