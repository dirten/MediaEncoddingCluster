#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/Frame.h"

#include "org/esb/util/Log.h"


#include "boost/shared_ptr.hpp"
using namespace org::esb::av;
using namespace org::esb::io;

int main(int argc, char **argv) {
  std::string src;
  if (argc == 1) {
    src = MEC_SOURCE_DIR;
    src.append("/test.dvd");
  } else {
    src = argv[1];
  }
  int stream_index = 0;
  if (argc == 3) {
    stream_index = atoi(argv[2]);
  }

  File f(src.c_str());
  FormatInputStream fis(&f);
  AVCodecContext * c = fis.getFormatContext()->streams[stream_index]->codec;
  Decoder *dec;
  if (true) {
    dec = new Decoder(c);
  } else {
    std::cout << "CodecId" << c->codec_id << std::endl;
    dec = new Decoder(c->codec_id);
    //  Decoder dec(c);
    //  dec->ctx->debug=1;
    //  dec.setChannels(c->channels);
    dec->setChannels(2);
    dec->setBitRate(c->bit_rate);
    dec->setPixelFormat(c->pix_fmt);
    dec->setTimeBase(c->time_base);
    dec->setWidth(c->width);
    dec->setHeight(c->height);
    dec->ctx->request_channel_layout = 2;
    /*
        dec->ctx->antialias_algo = c->antialias_algo;
        dec->ctx->b_frame_strategy = c->b_frame_strategy;
        dec->ctx->b_quant_factor = c->b_quant_factor;
        dec->ctx->b_quant_offset = c->b_quant_offset;
        dec->ctx->b_sensitivity = c->b_sensitivity;
        dec->ctx->bframebias = c->bframebias;
            dec->setFlag(CODEC_FLAG_GLOBAL_HEADER);
        dec->ctx->max_b_frames = c->max_b_frames;
        dec->ctx-> coder_type = c->coder_type;
        //	dec->ctx->flags=c->flags;
        dec->ctx->flags2 = c->flags2;
        dec->ctx->keyint_min = c->keyint_min;
        dec->ctx->gop_size = c->gop_size;
        dec->ctx->rc_buffer_size = c->rc_buffer_size;
        dec->ctx->rc_max_rate = c->rc_max_rate;
        dec->ctx->rc_initial_buffer_occupancy = c->rc_initial_buffer_occupancy;
        dec->ctx->crf = c->crf;
        dec->ctx->cqp = c->cqp;
        dec->ctx->scenechange_threshold = c->scenechange_threshold;
        dec->ctx->deblockalpha = c->deblockalpha;
        dec->ctx->deblockbeta = c->deblockbeta;
        dec->ctx->qmin = c->qmin;
        dec->ctx->qmax = c->qmax;
        dec->ctx->max_qdiff = c->max_qdiff;
        dec->ctx->qcompress = c->qcompress;
        dec->ctx->qblur = c->qblur;
        dec->ctx->complexityblur = c->complexityblur;
        dec->ctx->refs = c->refs;
        dec->ctx->sample_aspect_ratio = c->sample_aspect_ratio;
        dec->ctx->time_base = c->time_base;
        dec->ctx->partitions = c->partitions;
        dec->ctx->directpred = c->directpred;
        dec->ctx->me_method = c->me_method;
        dec->ctx->me_range = c->me_range;
        dec->ctx->me_subpel_quality = c->me_subpel_quality;
        dec->ctx->me_cmp = c->me_cmp;
        dec->ctx->trellis = c->trellis;
        dec->ctx->noise_reduction = c->noise_reduction;
        dec->ctx->level = c->level;
        dec->ctx->bit_rate_tolerance = c->bit_rate_tolerance;
        dec->ctx->i_quant_factor = c->i_quant_factor;
        dec->ctx->b_quant_factor = c->b_quant_factor;
        dec->ctx->chromaoffset = c->chromaoffset;
        dec->ctx->thread_count = c->thread_count;
        dec->ctx->coded_width = c->coded_width;
        dec->ctx->coded_height = c->coded_height;
     */
    std::cout << "Codec Extradata Size:" << c->extradata_size << std::endl;
    std::cout << "Codec Extradata Size:" << dec->ctx->extradata_size << std::endl;
    //	std::cout << "Codec Extradata Size:"<<c->extradata<<std::endl;
    //	std::cout << "Codec Extradata Size:"<<dec->ctx->extradata<<std::endl;
    dec->ctx->extradata = static_cast<uint8_t*> (av_malloc(c->extradata_size));
    //    memset(dec->ctx->extradata,0,c->extradata_size);
    memcpy(dec->ctx->extradata, c->extradata, c->extradata_size);
    av_free(c->extradata);
    //    dec->ctx->extradata = c->extradata;
    //	dec->ctx->extradata=static_cast<uint8_t*>(av_malloc(40));
    dec->ctx->extradata_size = c->extradata_size;
    std::cout << "Stream Decoder Flags " << c->flags << std::endl;
    std::cout << "Own Decoder Flags " << dec->ctx->flags << std::endl;
  }
//  dec->open();
  std::cout << "Codec Extradata Size:" << c->extradata_size << std::endl;
  std::cout << "Codec Extradata Size:" << dec->ctx->extradata_size << std::endl;
  std::cout << "Codec Extradata Size:" << c->extradata << std::endl;
  std::cout << "Codec Extradata Size:" << dec->ctx->extradata << std::endl;
  std::cout << "Stream Decoder Flags " << c->flags << std::endl;
  std::cout << "Own Decoder Flags " << dec->ctx->flags << std::endl;

  Encoder * enc = new Encoder(CODEC_ID_H264);
  enc->setChannels(2);
  enc->setBitRate(1024000);
  //  enc.setSampleRate(44100);
  //    enc->setPixelFormat(dec->getPixelFormat());
  AVRational ar;
  ar.num = 1;
  ar.den = 25;
  enc->setGopSize(100);
  enc->setTimeBase(ar);
  enc->setWidth(320);
  enc->setHeight(240);
  //	enc->ctx->debug=5;
  //	enc->ctx->bits_per_raw_sample=c->bits_per_raw_sample;
  //	enc->ctx->chroma_sample_location=c->chroma_sample_location;
  //      enc->setFlag(CODEC_FLAG_GLOBAL_HEADER);
  //     enc->setPixelFormat(PIX_FMT_YUV420P);
  //  enc.ctx->bits_per_raw_sample=dec.ctx
  enc->open();
  //  return 0;
  PacketInputStream pis(&fis);
  if (true) {
    Packet * p = NULL;
//    for (int a = 0; a < 100; a++) {
    for (int a = 0; (p = pis.readPacket()) != NULL; a++) {
      //  std::cerr<<"PacketLoop"<<std::endl;
//      if ((p = pis.readPacket()) != NULL) {
        if (false&&p->getStreamIndex() == stream_index) {
          Frame * f = dec->decode2(*p);
          if (f->isFinished()) {
            std::cerr << "frame finished" << std::endl;
            Packet pout = enc->encode(*f);
            std::cerr << "Packet size:" << pout.packet->size << std::endl;
          } else {
            std::cerr << "frame not finished" << std::endl;
          }
          delete f;
        }
        delete p;
//      }
    }
  }
  if (false) {
    Packet p;
//    for (int a = 0; a < 100; a++) {
    for (int a = 0; pis.readPacket(p) >= 0; a++) {
        //std::cerr<<"PacketLoop"<<std::endl;
      boost::shared_ptr<Packet> pPtr(new Packet(p));
//      if ((pis.readPacket(p)) >= 0) {
        if (false&&p.getStreamIndex() == stream_index) {
          Frame * f = dec->decode2(p);
          if (f->isFinished()) {
            std::cerr << "frame finished" << std::endl;
            Packet pout = enc->encode(*f);
            std::cerr << "Packet size:" << pout.packet->size << std::endl;
          } else {
            std::cerr << "frame not finished" << std::endl;
          }
          delete f;
        }
        //      delete p;
//      }
    }
  }
  delete dec;
  delete enc;
  Log::close();
}
