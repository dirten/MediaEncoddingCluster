MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1


#noinst_HEADERS=*.h 

lib_LTLIBRARIES = libav.la
SOURCES = \
	    Init.cpp \
            FrameConverter.cpp \
            FrameOutputStream.cpp \
            FormatInputStream.cpp \
            FormatBaseStream.cpp \
            FormatOutputStream.cpp \
            PacketInputStream.cpp \
            PacketOutputStream.cpp \
            Codec.cpp \
            Decoder.cpp \
            Encoder.cpp \
            Packet.cpp \
            Frame.cpp \
	        PPSWriter.cpp \
	        AVInputStream.cpp 
#	        VideoFrame.cpp \
#	        AudioFrame.cpp
            
#           CodecInputStream.cpp \
#            CodecOutputStream.cpp 
#            FrameInputStream.cpp 
            
libav_la_SOURCES = $(SOURCES)
libav_la_LDFLAGS = -release $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
libav_la_LIBADD = ../io/libio.la -lavformat -lavutil -lavcodec -lswscale
# set the include path found by configure
INCLUDES=   -I$(top_srcdir)/src
#	    $(all_includes) 

#noinst_HEADERS = *.h 
