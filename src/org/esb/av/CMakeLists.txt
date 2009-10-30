
include_directories (${MEC_SOURCE_DIR})
include_directories (${BOOST_INCLUDE_DIRS})

include_directories (${FFMPEG_INCLUDE_DIR})

LINK_DIRECTORIES(${FFMPEG_LIB_DIR})

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
  AVInputStream.cpp
  FormatStreamFactory.cpp
  StreamInfo.cpp
)
