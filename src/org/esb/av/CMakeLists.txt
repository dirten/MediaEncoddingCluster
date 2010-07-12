
include_directories (${MEC_SOURCE_DIR})
include_directories (${BOOST_INCLUDE_DIRS})

include_directories (${FFMPEG_INCLUDE_DIR})

LINK_DIRECTORIES(${FFMPEG_LIB_DIR})
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libogg/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/x264/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/xvidcore/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/lame/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libtheora/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libvorbis/lib)
add_library (av 
  Init.cpp 
  FrameConverter.cpp 
  FrameOutputStream.cpp 
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
#  PGMUtil.cpp
  AVInputStream.cpp
  FormatStreamFactory.cpp
  StreamInfo.cpp
)
TARGET_LINK_LIBRARIES(av
  ${FFMPEG_AVFORMAT_LIBRARY}
  ${FFMPEG_AVCODEC_LIBRARY}
  ${FFMPEG_AVUTIL_LIBRARY}
  ${FFMPEG_SWSCALE_LIBRARY}
  ${BZ2_LIB}
)
IF(NOT WIN32)
#	target_link_libraries (av x264 pthread vorbis vorbisenc ogg xvidcore mp3lame theora theoraenc theoradec z )
	target_link_libraries (av pthread z )
ENDIF(NOT WIN32)

IF(BUILD_SHARED_LIBS)
  INSTALL(TARGETS av
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
ENDIF(BUILD_SHARED_LIBS)