
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/Frame.h"
//#include "org/esb/lang/Thread.h"
#include "org/esb/av/FrameFormat.h"
#include "org/esb/av/FrameConverter.h"
#include <string>
#include <vector>
#include "erl.cpp"
//#include <stdio.h>
using namespace org::esb::av;

Packet * create_packet(ETERM* in) {
  ETERM * streamidx = erl_element(1, in);
//  ETERM * isKey = erl_element(2, in);
  ETERM * pts = erl_element(3, in);
  ETERM * dts = erl_element(4, in);
  ETERM * flags = erl_element(5, in);
  ETERM * duration = erl_element(6, in);
  ETERM * size = erl_element(7, in);
  ETERM * data = erl_element(8, in);
  Packet * p = new Packet(ERL_INT_UVALUE(size));
  p->packet->stream_index = ERL_INT_UVALUE(streamidx);
  sscanf((const char *)ERL_ATOM_PTR(pts),"%llu",&p->packet->pts);
  sscanf((const char *)ERL_ATOM_PTR(dts),"%llu",&p->packet->dts);
//  p->packet->dts = ERL_INT_UVALUE(dts);
  p->packet->flags = ERL_INT_VALUE(flags);
  p->packet->duration = ERL_INT_UVALUE(duration);
  memcpy(p->packet->data, ERL_BIN_PTR(data), p->getSize());
  return p;
}

/*
    ar & _codec_id;
    ar & _mode;
    ar & _flags;
    ar & _pix_fmt;
    ar & _width;
    ar & _height;
    ar & _time_base.num;
    ar & _time_base.den;
    ar & _gop_size;
    ar & _bit_rate;
    ar & _channels;
    ar & _sample_rate;
    ar & _sample_format;

 */
Decoder * create_decoder(ETERM* in) {
  ETERM * codecid = erl_element(6, in);
  ETERM * bitrate = erl_element(8, in);
  ETERM * rate = erl_element(9, in);
  ETERM * num = erl_element(10, in);
  ETERM * den = erl_element(11, in);
  ETERM * width = erl_element(12, in);
  ETERM * height = erl_element(13, in);
  ETERM * channels = erl_element(14, in);
  ETERM * gop = erl_element(15, in);
  ETERM * fmt = erl_element(16, in);

  Decoder * d = new Decoder(static_cast<CodecID> (ERL_INT_UVALUE(codecid)));
  d->findCodec(Codec::DECODER);
  d->setPixelFormat(d->_codec->type == CODEC_TYPE_VIDEO ? static_cast<PixelFormat> (ERL_INT_UVALUE(fmt)) : static_cast<PixelFormat> (0));
  //	d->setPixelFormat((PixelFormat)0);
  d->setHeight(ERL_INT_UVALUE(height));
  d->setWidth(ERL_INT_UVALUE(width));
  AVRational r;
  r.num = ERL_INT_UVALUE(num);
  r.den = ERL_INT_UVALUE(den);
  d->setTimeBase(r);
//  d->_time_base.num=ERL_INT_UVALUE(num);
//  d->_time_base.den=ERL_INT_UVALUE(den);
  d->setGopSize(ERL_INT_UVALUE(gop));
  d->setBitRate(ERL_INT_UVALUE(bitrate));
  d->setChannels(ERL_INT_UVALUE(channels));
  d->setSampleRate(d->_codec->type == CODEC_TYPE_AUDIO ? ERL_INT_UVALUE(rate) : 0);
  d->setSampleFormat(d->_codec->type == CODEC_TYPE_AUDIO ? static_cast<SampleFormat> (ERL_INT_UVALUE(fmt)) : static_cast<SampleFormat> (1));
  d->open();

  return d;
}

Encoder * create_encoder(ETERM* in) {
  ETERM * codecid = erl_element(6, in);
  ETERM * bitrate = erl_element(8, in);
  ETERM * rate = erl_element(9, in);
  ETERM * num = erl_element(10, in);
  ETERM * den = erl_element(11, in);
  ETERM * width = erl_element(12, in);
  ETERM * height = erl_element(13, in);
  ETERM * channels = erl_element(14, in);
  ETERM * gop = erl_element(15, in);
  ETERM * fmt = erl_element(16, in);

  Encoder * d = new Encoder(static_cast<CodecID> (ERL_INT_UVALUE(codecid)));
  d->findCodec(Codec::ENCODER);
  d->setPixelFormat(d->_codec->type == CODEC_TYPE_VIDEO ? static_cast<PixelFormat> (ERL_INT_UVALUE(fmt)) : static_cast<PixelFormat> (0));
  d->setHeight(ERL_INT_UVALUE(height));
  d->setWidth(ERL_INT_UVALUE(width));
  AVRational r;
  r.num = ERL_INT_UVALUE(num);
  r.den = ERL_INT_UVALUE(den);
  d->setTimeBase(r);
//  d->_time_base.num=ERL_INT_UVALUE(num);
//  d->_time_base.den=ERL_INT_UVALUE(den);
//  d->setFlag(0);
  d->setGopSize(ERL_INT_UVALUE(gop));
  d->setBitRate(ERL_INT_UVALUE(bitrate));
  d->setChannels(ERL_INT_UVALUE(channels));
  d->setSampleRate(ERL_INT_UVALUE(rate));
  d->setSampleFormat(d->_codec->type == CODEC_TYPE_AUDIO ? static_cast<SampleFormat> (ERL_INT_UVALUE(fmt)) : static_cast<SampleFormat> (0));
  d->open();

  return d;
}

ETERM * encode(ETERM* in) {
  bool toDebug=false;
  std::vector<ETERM *> terms;
  ETERM * input_term = erl_element(2, in);
//  ETERM * file_name = erl_element(1, input_term);
  ETERM * decoder = erl_element(3, input_term);
  ETERM * encoder = erl_element(4, input_term);
  ETERM * packet_list = erl_element(5, input_term);
/*
  logdebug("PacketList");
  erl_print_term((FILE*)stderr, packet_list);
  logdebug("Decoder");
  erl_print_term((FILE*)stderr, decoder);
  logdebug("Encoder");
  erl_print_term((FILE*)stderr, encoder);
*/
  //  org::esb::lang::Thread::sleep2(20000);
  Decoder *d = create_decoder(decoder);
  Encoder *e = create_encoder(encoder);
  FrameFormat * in_format=new FrameFormat();
  in_format->pixel_format = (PixelFormat) d->getPixelFormat(); //PIX_FMT_YUV420P;
  in_format->height = d->getHeight();
  in_format->width = d->getWidth();
  in_format->channels = d->getChannels();
  in_format->samplerate = d->getSampleRate();
  if (toDebug)
    logdebug("Input Formater created");
  FrameFormat * out_format= new FrameFormat();
  out_format->pixel_format = (PixelFormat) e->getPixelFormat(); //PIX_FMT_YUV420P;
  out_format->height = e->getHeight();
  out_format->width = e->getWidth();
  out_format->channels = e->getChannels();
  out_format->samplerate = e->getSampleRate();
  if (toDebug)
    logdebug("Output Formater created");
  FrameConverter * conv=new FrameConverter(*in_format, *out_format);
  if (toDebug)
    logdebug("Converter created");
//  d->toString();
//  e->toString();
  int pc = erl_length(packet_list);
  ETERM * tail=packet_list;
  for (int a = 0; a < pc; a++) {
//    logdebug("Output term " << a);
//    erl_print_term((FILE*)stderr, erl_element(a + 1, packet_list));
  if (toDebug)
    logdebug("Create Packet" << a);
    ETERM * head=erl_hd(tail);
    Packet *p = buildPacketFromTerm(head);
    tail=erl_tl(tail);
    if (toDebug)
      p->toString();
    Frame f = d->decode(*p);
    delete p;
//    f.toString();
    if (f._buffer == 0) {
  if (toDebug)
      logdebug("Frame Buffer == 0||f.getSize()<0");
      continue;
    }
    if (toDebug)
      logdebug("Frame Buffer > 0");

    Frame f2 = conv->convert(f);
    if (toDebug)
      logdebug("Frame Converted");

    f2.setPts(f2.getDts());
  if (toDebug)
    logdebug("after converting frame");
//    f2.toString();
    Packet ret=e->encode(f2);
//    ret.packet->pts = av_rescale_q(ret.packet->pts, d->getTimeBase(), e->getTimeBase());
//    ret.packet->dts = av_rescale_q(ret.packet->dts, d->getTimeBase(), e->getTimeBase());
    ETERM * pac=buildTermFromPacket(ret);
    terms.push_back(pac);
  if (toDebug)
    logdebug("Encoded PacketSize"<<ret.getSize());

    //		terms.push_back(erl_mk_uint(ERL_TUPLE_SIZE(erl_element(2, packet_list))));
  }
  delete d;
  delete e;
  delete out_format;
  delete in_format;
  delete conv;
//  terms.push_back(encoder);
//  terms.push_back(erl_mk_atom("encoded"));

  //  terms.push_back(erl_element(1, packet_list));
/*
  terms.push_back(erl_element(1, erl_element(1, packet_list)));
  terms.push_back(erl_element(2, erl_element(1, packet_list)));
  terms.push_back(erl_element(3, erl_element(1, packet_list)));
  terms.push_back(erl_element(4, erl_element(1, packet_list)));
  terms.push_back(erl_element(5, erl_element(1, packet_list)));
  terms.push_back(erl_element(6, erl_element(1, packet_list)));
  terms.push_back(erl_element(7, erl_element(1, packet_list)));
*/
  //  terms.push_back(erl_element(8,erl_element(1, packet_list)));
  //  terms.push_back(erl_mk_uint(ERL_TUPLE_SIZE(packet_list)));
  //  terms.push_back(decoder);
  //  terms.push_back(  in);
  /*
    terms.push_back(erl_mk_uint(ERL_TUPLE_SIZE(erl_element(1, packet_list))));
    terms.push_back(erl_mk_uint(ERL_IS_TUPLE(packet_list)));
    terms.push_back(erl_mk_uint(ERL_TUPLE_SIZE(packet_list)));*/
  return vector2list(terms);
}

int i=0;
int main() {
//  recur();

#ifdef WIN32
  /* Attention Windows programmers: you need to explicitly set
   * mode of stdin/stdout to binary or else the port program won't work
   */
  setmode(fileno(stdout), O_BINARY);
  setmode(fileno(stdin), O_BINARY);
#endif


  erl_init(NULL, 0);
  ETERM *intuple = NULL, *outtuple = NULL;
  byte * buf = new byte[50000000];
//  memset(buf,0,5000000);

  av_register_all();
  avcodec_init();
  avcodec_register_all();
  av_log_level=AV_LOG_ERROR;
  while (read_cmd(buf) > 0) {
    intuple = erl_decode(buf);
//    erl_print_term((FILE*)stderr, intuple);
//    logdebug("Term Size:"<<erl_size(erl_element(5,erl_element(2,intuple))));
//    return 0;
    ETERM* fnp = erl_element(1, intuple);
    if (fnp != NULL) {
      std::string func = (const char*) ERL_ATOM_PTR(fnp);
      if (func == "encode") {
        outtuple = encode(intuple);
      } else {
        std::vector<ETERM *> terms;
        terms.push_back(erl_mk_atom("unknown_command"));
        outtuple = vector2term(terms);
      }
//    erl_print_term((FILE*)stderr, outtuple);

      if (outtuple != NULL) {
        erl_encode(outtuple, buf);
        write_cmd(buf, erl_term_len(outtuple));
//        erl_free_compound(outtuple);
        erl_free_compound(outtuple);
        outtuple = NULL;
      }
    }
    erl_free_compound(intuple);
    erl_free_compound(fnp);
  //  delete fnp;
  }
  delete []buf;
}
