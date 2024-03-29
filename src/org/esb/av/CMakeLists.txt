
include_directories (${MEC_SOURCE_DIR})
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${LITESQL_DIR}/include)

include_directories (${FFMPEG_INCLUDE_DIR})

#ADD_DEFINITIONS()
set(CMAKE_CXX_FLAGS "-Wdeprecated-declarations")

#LINK_DIRECTORIES(${FFMPEG_LIB_DIR})
add_library (av SHARED
  Init.cpp 
  FrameConverter.cpp
#  FrameOutputStream.cpp 
  FormatInputStream.cpp 
  FormatBaseStream.cpp 
  FormatOutputStream.cpp 
  PacketInputStream.cpp 
  PacketOutputStream.cpp 
  Codec.cpp 
  Decoder.cpp 
  Encoder.cpp 
  Packet.cpp 
  Frame.cpp 
  PPSWriter.cpp
  PGMUtil.cpp
  BMPUtil.cpp
  CodecPropertyTransformer.cpp
  FormatStreamFactory.cpp
  StreamInfo.cpp
  FrameProcessor.cpp
  MediaLocator.cpp
  DataSource.cpp
  Rational.cpp
  TimeStamp.cpp
  Buffer.cpp
  Format.cpp
#  Duration.cpp
  Demultiplexer.cpp
  Track.cpp
  CCodec.cpp
  PixelFormatConverter.cpp
  ResizeFilter.cpp
  DeinterlaceFilter.cpp
  FrameRateFilter.cpp
#  TestFFMpegUrl.cpp
  AVPipe.cpp
  Resampler.cpp

  #Building all the filters
  AVFilter.cpp
  filter/BFrameProcessUnitFilter.cpp
  filter/MaxPacketCountFilter.cpp
)
TARGET_LINK_LIBRARIES(av
  ${FFMPEG_AVFILTER_LIBRARY}
  ${FFMPEG_AVFORMAT_LIBRARY}
  ${FFMPEG_AVCODEC_LIBRARY}
  ${FFMPEG_SWSCALE_LIBRARY}
  ${FFMPEG_SWRESAMPLE_LIBRARY}
  ${FFMPEG_AVUTIL_LIBRARY}
#  ${FFMPEG_AVCORE_LIBRARY}
#  signal
  util
  io
  mp3lame
  x264
  vpx
  theora
  ogg
  vorbis
  vorbisenc
  postproc
)
IF(NOT WIN32)
#	target_link_libraries (av x264 pthread vorbis vorbisenc ogg xvidcore mp3lame theora theoraenc theoradec z )
  target_link_libraries (av z)
ENDIF(NOT WIN32)

SET_TARGET_PROPERTIES( av PROPERTIES PREFIX "libmhive-")

  INSTALL(TARGETS av
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
#    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
