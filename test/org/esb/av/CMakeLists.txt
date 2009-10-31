ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)

include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_DIR}/include)
include_directories (${MEC_SOURCE_DIR}/src)

LINK_DIRECTORIES(${FFMPEG_DIR}/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libogg/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/x264/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/xvidcore/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/lame/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libtheora/lib)
LINK_DIRECTORIES    (${MEC_SOURCE_DIR}/source/build/libvorbis/lib)

add_executable (TestPacket TestPacket.cpp)
add_executable (TestSerializePacket TestSerializePacket.cpp)
add_executable (TestDeSerializePacket TestDeSerializePacket.cpp)
add_executable (TestFormatInputStream TestFormatInputStream.cpp)
add_executable (TestPacketInputStream TestPacketInputStream.cpp)
add_executable (TestPacketOutputStream TestPacketOutputStream.cpp)
add_executable (TestProcessAudioPacket TestProcessAudioPacket.cpp)
add_executable (TestFfmpeg TestFfmpeg.cpp)
add_executable (TestEuclid TestEuclid.cpp)
add_executable (TestFullTranscoding TestFullTranscoding.cpp)

target_link_libraries (TestPacket io av util lang avformat avcodec avutil swscale )
target_link_libraries (TestFfmpeg    ${FFMPEG_AVFORMAT_LIBRARY}
  ${FFMPEG_AVCODEC_LIBRARY}
  ${FFMPEG_AVUTIL_LIBRARY}
  ${FFMPEG_SWSCALE_LIBRARY} )
target_link_libraries (TestSerializePacket io av util lang avformat avcodec avutil swscale ${BOOST_SERIAL_LIB_MT} )
target_link_libraries (TestDeSerializePacket io av util lang avformat avcodec avutil swscale ${BOOST_SERIAL_LIB_MT})
target_link_libraries (TestFormatInputStream io av util lang avformat avcodec avutil swscale )
target_link_libraries (TestPacketInputStream io av util lang avformat avcodec avutil swscale )
target_link_libraries (TestPacketOutputStream io av util lang avformat avcodec avutil swscale )
target_link_libraries (TestProcessAudioPacket io av util lang avformat avcodec avutil swscale ${BOOST_SERIAL_LIB_MT})
target_link_libraries (TestFullTranscoding io av util lang avformat avcodec avutil swscale )

IF(NOT WIN32)
target_link_libraries (TestFfmpeg ${BZ2_LIB} x264 pthread z mp3lame theora vorbis vorbisenc xvidcore ogg)
target_link_libraries (TestPacket ${BZ2_LIB} avformat avcodec avutil swscale  x264 pthread z mp3lame theora vorbis vorbisenc xvidcore ogg)
target_link_libraries (TestSerializePacket ${BZ2_LIB} avformat avcodec avutil swscale  x264 pthread z mp3lame theora vorbis vorbisenc xvidcore ogg ${BOOST_SERIAL_LIB_MT})
target_link_libraries (TestDeSerializePacket ${BZ2_LIB} avformat avcodec avutil swscale  x264 pthread z mp3lame theora vorbis vorbisenc xvidcore ogg ${BOOST_SERIAL_LIB_MT})
target_link_libraries (TestFormatInputStream ${BZ2_LIB} avformat avcodec avutil swscale  x264 pthread z mp3lame theora vorbis vorbisenc xvidcore ogg)
target_link_libraries (TestPacketInputStream ${BZ2_LIB} avformat avcodec avutil swscale  x264 pthread z mp3lame theora vorbis vorbisenc xvidcore ogg)
target_link_libraries (TestPacketOutputStream ${BZ2_LIB} avformat avcodec avutil swscale  x264 pthread z mp3lame theora vorbis vorbisenc xvidcore ogg)
target_link_libraries (TestProcessAudioPacket ${BZ2_LIB} avformat avcodec avutil swscale  x264 pthread z mp3lame theora vorbis vorbisenc xvidcore ogg ${BOOST_SERIAL_LIB_MT})
target_link_libraries (TestFullTranscoding ${BZ2_LIB} avformat avcodec avutil swscale  x264 pthread z mp3lame theora vorbis vorbisenc xvidcore ogg ${BOOST_SERIAL_LIB_MT})

ENDIF(NOT WIN32)

ADD_TEST(LibAVPacket ${EXECUTABLE_OUTPUT_PATH}/TestPacket)
ADD_TEST(TestFfmpeg ${EXECUTABLE_OUTPUT_PATH}/TestFfmpeg )
ADD_TEST(LibAVSerializePacket ${EXECUTABLE_OUTPUT_PATH}/TestSerializePacket)
ADD_TEST(LibAVDeSerializePacket ${EXECUTABLE_OUTPUT_PATH}/TestDeSerializePacket)
ADD_TEST(LibAVFormatInputStream ${EXECUTABLE_OUTPUT_PATH}/TestFormatInputStream)
ADD_TEST(LibAVPacketInputStream ${EXECUTABLE_OUTPUT_PATH}/TestPacketInputStream)
ADD_TEST(TestPacketOutputStream ${EXECUTABLE_OUTPUT_PATH}/TestPacketOutputStream)
ADD_TEST(TestProcessAudioPacket ${EXECUTABLE_OUTPUT_PATH}/TestProcessAudioPacket)
ADD_TEST(TestFullTranscoding ${EXECUTABLE_OUTPUT_PATH}/TestProcessAudioPacket)


