extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavcodec/opt.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libavutil/fifo.h>

}
#include <iostream>

int main(int argc, char ** argv) {
  av_register_all();
  if (argc < 3) {
    return 0;
  }
  /*opening input file*/
  AVFormatContext * formatCtx;// = avformat_alloc_context();
//  AVInputFormat *file_iformat = av_find_input_format("mpeg2ts");
//  AVFormatParameters params, *ap = &params;
//  memset(ap, 0, sizeof (*ap));
//  ap->prealloced_context = 1;

  char * filename = argv[1];
  int sid = atoi(argv[2]);
  if (av_open_input_file(&formatCtx, filename, NULL, 0, NULL) != 0) {
    std::cout << "could not open file" << filename << std::endl;
    return 0;
  }
//  formatCtx->debug = 5;
  if (av_find_stream_info(formatCtx) < 0) {
    std::cout << "could not read stream info " << filename << std::endl;
    return 0;
  }

  /*
   * Create and open Decoder
   */
  AVCodecContext * dec = formatCtx->streams[sid]->codec;
  AVCodec * codec = avcodec_find_decoder(dec->codec_id);
//  std::cout <<"Option:"<<(*(AVClass**)dec)->option->name<<std::endl;
//  av_opt_show(dec, NULL);
  if (codec == NULL)
    std::cout << "Decoder not found" << std::endl;
  if (avcodec_open(dec, codec) < 0) {
    std::cout << "Decoder not opened" << std::endl;
  }
  /*
   * Create and open Encoder
   */
  AVCodecContext * enc = avcodec_alloc_context();
  enc->codec_id = CODEC_ID_H264;
  enc->codec_type = AVMEDIA_TYPE_VIDEO;
  enc->bit_rate = 1024000;
  enc->gop_size = 12;
  enc->max_b_frames = 2;
  enc->height = 240;
  enc->width = 320;
  enc->me_range = 16;
  enc->max_qdiff = 4;
  enc->qmin = 10;
  enc->qmax = 51;
  enc->qcompress = 0.6;
  enc->thread_count = 1;
  AVCodec * codeco = avcodec_find_encoder(enc->codec_id);
  if (codeco == NULL)
    std::cout << "Encoder not found" << std::endl;
  if (avcodec_open(enc, codeco) < 0) {
    std::cout << "Encoder not opened" << std::endl;
  }
  std::cout << "EncDelay" << enc->delay << std::endl;
  std::cout << "DecDelay" << dec->delay << std::endl;
  int out=0;
  for (int i = 0; true;) {
    AVPacket pkt;
    int ret = av_read_frame(formatCtx, &pkt);
    if (pkt.stream_index == sid&&pkt.dts>=3760262033) {
      /*try to decode*/
      std::cout << "PacketDts:"<<pkt.dts<<std::endl;
      int fin = 0;
      AVFrame pic;
      avcodec_get_frame_defaults(&pic);
      int bytes = avcodec_decode_video2(dec, &pic, &fin, &pkt);
      if (bytes < 0)
        std::cout << "Decoding failed" << std::endl;
      switch (pic.pict_type) {
        case FF_B_TYPE:
          std::cout << ("B");
          break;
        case FF_I_TYPE:
          std::cout << ("I");
          break;
        case FF_P_TYPE:
          std::cout << ("P");
          break;
        case FF_S_TYPE:
          std::cout << ("S");
          break;
        case FF_SI_TYPE:
          std::cout << ("SI");
          break;
        case FF_SP_TYPE:
          std::cout << ("SP");
          break;
        case FF_BI_TYPE:
          std::cout << ("BI");
          break;
        default:
          std::cout << "U:" << pic.pict_type;
          break;

      }
      out++;

      if (!fin) {
        std::cout << "frame not finished" << std::endl;
        continue;
      }
      
      std::cout << "frame finished" << std::endl;
      if(false){
      const int buffer_size = 1024 * 256;
      uint8_t data[buffer_size];
      memset(&data, 0, buffer_size);

      int ret = avcodec_encode_video(enc, (uint8_t*) & data, buffer_size, &pic);
      std::cout << "Packet data size :" << ret << std::endl;
      if (ret < 0)
        std::cout << "Encoding failed" << std::endl;
      }
      i++;
      if (i >= 15){
        std::cout << "Decoded Packets:"<<i<<std::endl;
        std::cout << "Decoded Frames:"<<out<<std::endl;

        break;
      }
      //			exit(0);
    }
    
    av_free_packet(&pkt);
  }
  
  std::cout << "Encoding last packets" << std::endl;
  bool have_more = true;
  while (have_more) {
    const int buffer_size = 1024 * 256;
    uint8_t data[buffer_size];
    memset(&data, 0, buffer_size);
    int ret = avcodec_encode_video(enc, (uint8_t*) & data, buffer_size, NULL);
    std::cout << "Packet data size :" << ret << std::endl;
//    std::cout << "Packet pts :" << enc->coded_frame->pts << std::endl;
    if (ret < 0) {
      std::cout << "Encoding failed" << std::endl;
    }
    if (ret == 0)
      have_more = false;
  }

//  av_freep(enc->stats_in);

  avcodec_close(enc);
  avcodec_close(dec);

  av_free(enc);
//  av_free(dec);

//  av_free(file_iformat);
  av_close_input_file(formatCtx);
//  av_free(formatCtx);
  return 0;
}
