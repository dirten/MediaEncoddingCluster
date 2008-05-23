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
INCLUDES=   -I$(top_srcdir)/src -I/usr/lib/erlang/lib/erl_interface-3.5.5.4/include

# the library search path.
erltest_LDADD=-L/usr/lib/erlang/lib/erl_interface-3.5.5.4/lib -lerl_interface -lei -lpthread -lstdc++ $(top_srcdir)/src/org/esb/av/libav.la
#		/home/jhoelscher/bripper/MediaEncodingCluster/src/org/esb/av/.libs/libav.a \
#		/home/jhoelscher/bripper/MediaEncodingCluster/src/org/esb/io/.libs/libio.a \
#		/home/jhoelscher/bripper/tnt/cxxtools/lib/libcxxtools.a \
#		/home/jhoelscher/bripper/ffmpeg-install/lib/libavformat.a \
#		/home/jhoelscher/bripper/ffmpeg-install/lib/libavcodec.a \
#		/home/jhoelscher/bripper/ffmpeg-install/lib/libavutil.a \
#		/home/jhoelscher/bripper/x264/lib/libx264.a \
#		/home/jhoelscher/bripper/ffmpeg-install/lib/libswscale.a -lz
