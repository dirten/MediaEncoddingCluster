/* 
 * File:   erlsys.cpp
 * Author: jhoelscher
 *
 * Created on 26. Februar 2009, 13:23
 */

//#include <stdlib.h>
#include "erl.cpp"
#include "org/esb/av/AV.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"

#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameFormat.h"
#include "org/esb/av/FrameConverter.h"

#include "org/esb/util/Log.h"
#include "org/esb/util/Decimal.h"

#include "org/esb/av/FormatStreamFactory.h"

/**********************************************************
*
*
***********************************************************/
ETERM * formatlist(ETERM * v) {
  std::vector<ETERM *> terms;
//  logdebug("Reading Format:");

  AVOutputFormat *ofmt = NULL;
  while ((ofmt = av_oformat_next(ofmt))) {
    std::vector<ETERM *> f;
    f.push_back(erl_mk_string(ofmt->name));
    f.push_back(erl_mk_string(ofmt->long_name));
    const char * mime_type = ofmt->mime_type ? ofmt->mime_type : "";
    const char * extensions = ofmt->extensions ? ofmt->extensions : "";
    f.push_back(erl_mk_string(mime_type));
    f.push_back(erl_mk_string(extensions));
    //    erl_print_term((FILE*)stderr, vector2term(f));
    terms.push_back(vector2term(f));
  }
  //  logdebug("Reading Format ready");
  //  erl_print_term((FILE*)stderr, vector2term(terms));
  return vector2list(terms);
}

ETERM * codeclist(ETERM * v) {
  std::vector<ETERM *> terms;
  AVCodec *p = NULL;
  while ((p = av_codec_next(p))) {
    std::vector<ETERM *> c;
    c.push_back(erl_mk_string(p->name));
    c.push_back(erl_mk_int(p->id));
    c.push_back(erl_mk_int(p->type));
    c.push_back(erl_mk_int(p->encode?1:0));
    c.push_back(erl_mk_int(p->decode?1:0));
    int cap=p->capabilities?p->capabilities:0;
    c.push_back(erl_mk_int(cap));
//    erl_print_term((FILE*)stderr, vector2term(c));
    terms.push_back(vector2term(c));
  }
  return vector2list(terms);
}

/**********************************************************
*
*
***********************************************************/
org::esb::av::PacketOutputStream * pos = NULL;
org::esb::av::FormatOutputStream *fos = NULL;

ETERM * createfile(ETERM* in) {
  ETERM *file = erl_element(2, in);
  org::esb::io::File fout((const char*) ERL_ATOM_PTR(file));
  logdebug("Create File:"<<(const char*) ERL_ATOM_PTR(file));
  fos = new org::esb::av::FormatOutputStream(&fout);
  pos = new org::esb::av::PacketOutputStream(fos);
  return erl_mk_string("ok");
}

ETERM * initfile(ETERM* in) {
  pos->init();
  return erl_mk_string("ok");
}

ETERM * closefile(ETERM* in) {
  pos->close();
  fos->close();
  return erl_mk_string("ok");
}

ETERM * addstream(ETERM * in) {
  ETERM *streamidx = erl_element(3, in);
  Decoder * d = buildDecoderFromTerm(erl_element(2, in));
  pos->setEncoder(*d, ERL_INT_UVALUE(streamidx));
  return erl_mk_string("ok");
}

ETERM * writepacket(ETERM * in) {
//  logdebug("WritePacket");
  Packet * p=buildPacketFromTerm(in);
  p->packet->pts=0;
  p->packet->dts=0;
  pos->writePacket(*p);
  delete p;
  return erl_mk_string("ok");
}

ETERM * writepacketlist(ETERM * in) {
  ETERM * packet_list = erl_element(2, in);
  ETERM * tail=packet_list;
  int pc = erl_length(packet_list);
//  logdebug("WritePacketList");

  for(int a = 0;a<pc;a++){
//  logdebug("WritePacket");
    ETERM * head=erl_hd(tail);
//    Packet *p = buildPacketFromTerm(head);
    writepacket(head);
    erl_free_compound(head);
    tail=erl_tl(tail);
  }


  return erl_mk_string("ok");
}



/**********************************************************
*
*
***********************************************************/
ETERM * streaminfo(ETERM * v) {
  std::vector<ETERM *> terms;
  ETERM *file = erl_element(2, v);
  ETERM *stream = erl_element(3, v);
  int s = ERL_INT_UVALUE(stream);
  File f((const char*) ERL_ATOM_PTR(file));
  if (f.exists()) {
    //    FormatInputStream fis(&f);
    FormatInputStream *fis = FormatStreamFactory::getInputStream(f.getPath());
    if (!fis->isValid() || s < 0 || s >= fis->getFormatContext()->nb_streams) {
      terms.push_back(erl_mk_atom("streamnotfound"));
    } else {

      AVStream *str = fis->getFormatContext()->streams[s];
      terms.push_back(erl_mk_int(0));
      terms.push_back(erl_mk_int(str->index));
      terms.push_back(erl_mk_int(str->codec->codec_type));
      terms.push_back(erl_mk_int(str->codec->codec_id));
      terms.push_back(erl_mk_int(str->codec->bit_rate));
      terms.push_back(erl_mk_float(str->codec->codec_type == CODEC_TYPE_VIDEO ? av_q2d(str->r_frame_rate) : str->codec->sample_rate));
      terms.push_back(erl_mk_int(str->time_base.num));
      terms.push_back(erl_mk_int(str->time_base.den));
      terms.push_back(erl_mk_int(str->codec->width));
      terms.push_back(erl_mk_int(str->codec->height));
      terms.push_back(erl_mk_int(str->codec->channels));
      terms.push_back(erl_mk_int(str->codec->gop_size));
      terms.push_back(erl_mk_int(str->codec->codec_type == CODEC_TYPE_VIDEO ? str->codec->pix_fmt : str->codec->sample_fmt));
      terms.push_back(erl_mk_string(Decimal(str->start_time).toString().c_str()));
      terms.push_back(erl_mk_string(Decimal(str->duration).toString().c_str()));
    }
  } else {
    terms.push_back(erl_mk_atom("filenotfound"));
  }
  return vector2term(terms);

}


ETERM * fileinfo(ETERM * v) {
  std::vector<ETERM *> terms;
  ETERM *argp = erl_element(2, v);
  std::string t((const char*) ERL_ATOM_PTR(argp));
  File f(t);
//  logdebug("fileinfo:"<<f.getPath());
  if (f.exists()) {
    //    FormatInputStream fis(&f);
    FormatInputStream *fis = FormatStreamFactory::getInputStream(f.getPath());
    if (!fis->isValid()) {
      terms.push_back(erl_mk_atom("format_invalid"));
    } else {
//      terms.push_back(erl_mk_string(f.getFilePath().c_str()));
//      terms.push_back(erl_mk_string(f.getFilePath().c_str()));
      terms.push_back(erl_mk_string("undefined"));
      terms.push_back(erl_mk_string("undefined"));
      terms.push_back(erl_mk_string(Decimal(fis->getFileSize()).toString().c_str()));
      terms.push_back(erl_mk_string(fis->getFormatContext()->iformat->name));
      terms.push_back(erl_mk_int(fis->getStreamCount()));
      terms.push_back(erl_mk_string(Decimal(fis->getFormatContext()->duration).toString().c_str()));
      terms.push_back(erl_mk_int(fis->getFormatContext()->bit_rate));
      terms.push_back(erl_mk_string(Decimal(fis->getFormatContext()->start_time).toString().c_str()));
//      terms.push_back(erl_mk_string(Decimal(fis->getFormatContext()->duration).toString().c_str()));
    }
  } else {
    terms.push_back(erl_mk_atom("filenotfound"));
  }
  return vector2term(terms);
}
ETERM * packetstream(ETERM * v) {
//  erl_print_term((FILE*)stderr,v);
  std::vector<ETERM *> terms;
  ETERM *file = erl_element(2, v);
  ETERM *stream = erl_element(3, v);
  ETERM *seek = erl_element(4, v);
  ETERM *count = erl_element(5, v);
  int str = ERL_INT_UVALUE(stream);
  int se = ERL_INT_VALUE(seek);
  int c = ERL_INT_VALUE(count);
  File f((const char*) ERL_ATOM_PTR(file));
  if (f.exists()) {
    long long int s;
//    logdebug("SeekValue:"<<erl_iolist_to_string(seek));
    sscanf((const char *)erl_iolist_to_string(seek),"%llu",&s);
//    logdebug("SeekValueScanned:"<<s);
    FormatInputStream *fis = FormatStreamFactory::getInputStream(f.getPath(), s);
    PacketInputStream pis(fis);
    Packet p;
//    if (se >= 0)
//      fis->seek(str, se);
    for(int a=0;a<c;a++){
      if(pis.readPacket(p)>=0){
        terms.push_back(buildTermFromPacket(p));
      }else{
        break;
      }
    }
  }
  return vector2list(terms);
}




/**********************************************************
*
*
***********************************************************/
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
  return vector2list(terms);
}






/*
 * 
 */
int main(int argc, char** argv) {
#ifdef WIN32
  /* Attention Windows programmers: you need to explicitly set
   * mode of stdin/stdout to binary or else the port program won't work
   */
  setmode(fileno(stdout), O_BINARY);
  setmode(fileno(stdin), O_BINARY);
#endif
  erl_init(NULL, 0);
  //  file_import();
  //  logdebug("Program End");
  //  return 0;
  av_register_all();
  avcodec_init();
  avcodec_register_all();

  ETERM *intuple = NULL, *outtuple = NULL;

  byte *buf = new byte[5000000];
  memset(buf, 0, 5000000);
  while (read_cmd(buf) > 0) {
    intuple = erl_decode(buf);
    //    std::cerr<<"InTermSize:"<<erl_size(intuple)<<std::endl;
    ETERM* fnp = erl_element(1, intuple);
    if (fnp != NULL) {
      std::string func = (const char*) ERL_ATOM_PTR(fnp);
/*Information Functions for FFMpeg*/
	  if (func == "formatlist") {
        outtuple = formatlist(intuple);
      } else
        if (func == "codeclist") {
        outtuple = codeclist(intuple);
      } else 
		if (func == "createfile") {
        outtuple = createfile(intuple);
      } else
/*System functions for creation output Files*/
		if (func == "addstream") {
        outtuple = addstream(intuple);
      } else
        if (func == "writepacket") {
        outtuple = writepacketlist(intuple);
      } else
        if (func == "initfile") {
        outtuple = initfile(intuple);
      } else
        if (func == "closefile") {
        outtuple = closefile(intuple);
      }else
/*System function to get Data from Input Files*/
	  if (func == "fileinfo") {
        outtuple = fileinfo(intuple);
      } else if (func == "streaminfo") {
        outtuple = streaminfo(intuple);
      } else if (func == "packetstream") {
        outtuple = packetstream(intuple);
      } else 
/*System function to Encode Frames*/
	  if (func == "encode") {
        outtuple = encode(intuple);
      } else{
        std::vector<ETERM *> terms;
        terms.push_back(erl_mk_atom("unknown_command"));
        outtuple = vector2term(terms);
      }
      if (intuple != NULL) {
        erl_free_compound(intuple);
        intuple = NULL;
      }

//      logdebug("try return data");
      if (outtuple != NULL) {
//        logdebug("return data");
        int size = erl_term_len(outtuple);
        if (size > 5000000) {
          logerror("OutTuple to big(max 5000000b):" << size);
        }
        erl_encode(outtuple, buf);
        write_cmd(buf, size);
        erl_free_compound(outtuple);
        outtuple = NULL;
      } else {
        logdebug("OutTuple is NULL");
      }
    }
  }
  delete []buf;
}

