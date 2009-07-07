/* 
 * File:   erlsys.cpp
 * Author: jhoelscher
 *
 * Created on 26. Februar 2009, 13:23
 */

#include <vector>


#define DEBUG true

//#include <stdlib.h>
//#include "org/esb/av/AV.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketOutputStream.h"


#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameFormat.h"
#include "org/esb/av/FrameConverter.h"

#include "org/esb/util/Log.h"
#include "org/esb/util/Decimal.h"

#include "org/esb/av/FormatStreamFactory.h"
#include "erl.cpp"
using namespace org::esb::util;

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
    f.push_back(erl_mk_int(ofmt->flags));
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
    if (p->encode) {
      std::vector<ETERM *> c;
      c.push_back(erl_mk_string(p->name));
      c.push_back(erl_mk_string(p->long_name));
      c.push_back(erl_mk_int(p->id));
      c.push_back(erl_mk_int(p->type));
      c.push_back(erl_mk_int(p->encode ? 1 : 0));
      c.push_back(erl_mk_int(p->decode ? 1 : 0));
      int cap = p->capabilities ? p->capabilities : 0;
      c.push_back(erl_mk_int(cap));
      //    erl_print_term((FILE*)stderr, vector2term(c));
      terms.push_back(vector2term(c));
    }
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
  std::vector<ETERM *> terms;
  ETERM *file = erl_element(2, in);
  org::esb::io::File fout((const char*) ERL_ATOM_PTR(file));
  logdebug("Create File:" << (const char*) ERL_ATOM_PTR(file));
  fos = new org::esb::av::FormatOutputStream(&fout);
  pos = new org::esb::av::PacketOutputStream(fos);

  terms.push_back(erl_mk_atom("ok_file_created"));
  return vector2list(terms);
}

ETERM * initfile(ETERM* in) {
  std::vector<ETERM *> terms;
  pos->init();
  terms.push_back(erl_mk_atom("ok_file_initialized"));
  return vector2list(terms);
}

ETERM * closefile(ETERM* in) {
  std::vector<ETERM *> terms;
  pos->close();
  fos->close();
  terms.push_back(erl_mk_atom("ok_file_closed"));

  return vector2list(terms);
}

ETERM * addstream(ETERM * in) {
  std::vector<ETERM *> terms;
  ETERM *streamidx = erl_element(3, in);
  Encoder * d = buildEncoderFromTerm(erl_element(2, in));
  pos->setEncoder(*d, ERL_INT_UVALUE(streamidx));
  d->setOutputStream(pos);
  terms.push_back(erl_mk_atom("ok_stream_added"));

  return vector2list(terms);
}

ETERM * writepacket(ETERM * in) {
  std::vector<ETERM *> terms;
  //  erl_print_term((FILE*)stderr, in);
  //  logdebug("WritePacket");
  //  logdebug("Build Packet:");
  Packet * p = buildPacketFromTerm(in);
  //  logdebug("Packet Ready:");
  //  p->toString();
  //  p->packet->pts = 0;
  //  p->packet->dts = 0;
  //  if (p->getSize() > 0) {
  pos->writePacket(*p);
  delete p;
  //  } else
  //    logerror("Packet is not usabel");
  terms.push_back(erl_mk_string("ok_packet_written"));
  return vector2list(terms);
}

ETERM * writepacketlist(ETERM * in) {
  std::vector<ETERM *> terms;

  ETERM * packet_list = erl_element(2, in);
  ETERM * tail = packet_list;
  int pc = erl_length(packet_list);
  //  logdebug("WritePacketList");

  for (int a = 0; a < pc; a++) {
    //  logdebug("WritePacket");
    ETERM * head = erl_hd(tail);
    //    Packet *p = buildPacketFromTerm(head);
    writepacket(head);
    //    erl_free_compound(head);
    tail = erl_tl(tail);
  }
  //  logdebug("WritePacket Ready- > returning");
  terms.push_back(erl_mk_string("ok_packets_written"));
  return vector2list(terms);
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
    std::string filename = f.getPath();
    FormatInputStream *fis = FormatStreamFactory::getInputStream(filename);
    if (!fis->isValid() || s < 0 || s >= fis->getStreamCount()) {
      terms.push_back(erl_mk_atom("streamnotfound"));
    } else {

      StreamInfo *str = fis->getStreamInfo(s);
      terms.push_back(erl_mk_int(0));
      terms.push_back(erl_mk_int(str->getIndex()));
      terms.push_back(erl_mk_int(str->getCodecType()));
      terms.push_back(erl_mk_int(str->getCodecId()));
      terms.push_back(erl_mk_int(str->getCodecBitrate()));
      terms.push_back(erl_mk_int(str->getCodecType() == CODEC_TYPE_VIDEO ? av_q2d(str->getFrameRate()) : str->getCodecSampleRate()));
      terms.push_back(erl_mk_int(str->getTimeBase().num));
      terms.push_back(erl_mk_int(str->getTimeBase().den));
      terms.push_back(erl_mk_int(str->getWidth()));
      terms.push_back(erl_mk_int(str->getheight()));
      terms.push_back(erl_mk_int(str->getChannels()));
      terms.push_back(erl_mk_int(str->getGOPSize()));
      terms.push_back(erl_mk_int(str->getCodecType() == CODEC_TYPE_VIDEO ? (int) str->getPixelFormat() : (int) str->getSampleFormat()));
      terms.push_back(erl_mk_string(Decimal(str->getFirstPts()).toString().c_str()));
      terms.push_back(erl_mk_string(Decimal(str->getDuration()).toString().c_str()));
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
  logdebug("fileinfo:" << f.getPath());
  if (f.exists()) {
    //    FormatInputStream fis(&f);
    std::string filename = f.getPath();
    FormatInputStream *fis = FormatStreamFactory::getInputStream(filename);
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
  //  ETERM *stream = erl_element(3, v);
  ETERM *seek = erl_element(4, v);
  ETERM *count = erl_element(5, v);
  //  int str = ERL_INT_UVALUE(stream);
  //  int se = ERL_INT_VALUE(seek);
  int c = ERL_INT_VALUE(count);
  File f((const char*) ERL_ATOM_PTR(file));
  if (f.exists()) {
    long long int s;
    //    logdebug("SeekValue:"<<erl_iolist_to_string(seek));
    sscanf((const char *) erl_iolist_to_string(seek), "%ll", &s);
    //    logdebug("SeekValueScanned:"<<s);
    std::string filename = f.getPath();
    FormatInputStream *fis = FormatStreamFactory::getInputStream(filename, s);
    PacketInputStream pis(fis);
    Packet p;
    //    if (se >= 0)
    //      fis->seek(str, se);
    for (int a = 0; a < c; a++) {
      if (pis.readPacket(p) >= 0) {
        //        logdebug("read_packet");
        terms.push_back(buildTermFromPacket(p));
      } else {
        terms.push_back(erl_mk_atom("eof"));
        break;
      }
    }
  } else {
    terms.push_back(erl_mk_atom("filenotfound"));
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

  Decoder * d = new Decoder(static_cast<CodecID > (ERL_INT_UVALUE(codecid)));
  d->findCodec(Codec::DECODER);
  d->setPixelFormat(d->_codec->type == CODEC_TYPE_VIDEO ? static_cast<PixelFormat > (ERL_INT_UVALUE(fmt)) : static_cast<PixelFormat > (0));
  //	d->setPixelFormat((PixelFormat)0);
  d->setHeight(ERL_INT_UVALUE(height));
  d->setWidth(ERL_INT_UVALUE(width));
  AVRational r;
  r.num = ERL_INT_UVALUE(num);
  r.den = ERL_INT_UVALUE(den);
  d->setTimeBase(r);
  d->setGopSize(ERL_INT_UVALUE(gop));
  d->setBitRate(ERL_INT_UVALUE(bitrate)*1000);
  d->setChannels(ERL_INT_UVALUE(channels));
  d->setSampleRate(d->_codec->type == CODEC_TYPE_AUDIO ? ERL_INT_UVALUE(rate) : 0);
  d->setSampleFormat(d->_codec->type == CODEC_TYPE_AUDIO ? static_cast<SampleFormat > (ERL_INT_UVALUE(fmt)) : static_cast<SampleFormat > (1));
  //  d->open();

  return d;
}

Encoder * create_encoder(ETERM* in) {
  //  erl_print_term((FILE*)stderr, in);
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
  ETERM * flags = erl_element(19, in);

  Encoder * d = new Encoder(static_cast<CodecID > (ERL_INT_UVALUE(codecid)));
  d->findCodec(Codec::ENCODER);
  d->setPixelFormat(d->_codec->type == CODEC_TYPE_VIDEO ? static_cast<PixelFormat > (ERL_INT_UVALUE(fmt)) : static_cast<PixelFormat > (0));
  d->setHeight(ERL_INT_UVALUE(height));
  d->setWidth(ERL_INT_UVALUE(width));
  AVRational r;
  r.num = ERL_INT_UVALUE(num);
  r.den = ERL_INT_UVALUE(den);
  d->setTimeBase(r);
  d->setGopSize(ERL_INT_UVALUE(gop));
  d->setBitRate(ERL_INT_UVALUE(bitrate)*1000);
  d->setChannels(ERL_INT_UVALUE(channels));
  d->setSampleRate(ERL_INT_UVALUE(rate));
  d->setSampleFormat(d->_codec->type == CODEC_TYPE_AUDIO ? static_cast<SampleFormat > (ERL_INT_UVALUE(fmt)) : static_cast<SampleFormat > (0));
  d->setFlag(CODEC_FLAG_PASS1);
  d->setFlag(ERL_INT_UVALUE(flags));
  //  d->open();

  return d;
}

ETERM * encode(ETERM* in) {
  bool toDebug = DEBUG;
  std::vector<ETERM *> terms;
  std::string statistics;
  ETERM * input_term = erl_element(2, in);
  ETERM * pass = erl_element(3, input_term);
  ETERM * decoder = erl_element(4, input_term);
  ETERM * encoder = erl_element(5, input_term);
  ETERM * packet_list = erl_element(6, input_term);
  if (toDebug) {
    //    logdebug("PacketList");
    //    erl_print_term((FILE*)stderr, packet_list);
    logdebug("Decoder");
    erl_print_term((FILE*) stderr, decoder);
    logdebug("Encoder");
    erl_print_term((FILE*) stderr, encoder);
  }
  //  org::esb::lang::Thread::sleep2(20000);
  int multipass = ERL_INT_UVALUE(pass);
  Decoder *d = create_decoder(decoder);
  Encoder *e = create_encoder(encoder);
  PacketTermSink sink; // = new PacketTermSink();
  e->setSink(&sink);
  d->ctx->request_channel_layout = e->getChannels();
  e->open();
  d->open();
  if (toDebug)
    logdebug("Decoder:" << d->toString());
  if (toDebug)
    logdebug("Encoder:" << e->toString());
  FrameFormat * in_format = new FrameFormat();
  in_format->pixel_format = (PixelFormat) d->getPixelFormat(); //PIX_FMT_YUV420P;
  in_format->height = d->getHeight();
  in_format->width = d->getWidth();
  in_format->channels = d->getChannels();
  in_format->samplerate = d->getSampleRate();
  if (toDebug)
    logdebug("Input Formater created");
  FrameFormat * out_format = new FrameFormat();
  out_format->pixel_format = (PixelFormat) e->getPixelFormat(); //PIX_FMT_YUV420P;
  out_format->height = e->getHeight();
  out_format->width = e->getWidth();
  out_format->channels = e->getChannels();
  out_format->samplerate = e->getSampleRate();
  if (toDebug)
    logdebug("Output Formater created");
  FrameConverter * conv = new FrameConverter(d, e);
  if (toDebug)
    logdebug("Converter created");
  //  d->toString();
  //  e->toString();
  int pc = erl_length(packet_list);
  ETERM * tail = packet_list;
  for (int a = 0; a < pc; a++) {
    //        logdebug("Output term " << a);
    //    erl_print_term((FILE*)stderr, erl_element(a + 1, packet_list));
    if (toDebug)
      logdebug("Create Packet" << a);
    ETERM * head = erl_hd(tail);
    Packet *p = buildPacketFromTerm(head);
    tail = erl_tl(tail);
    if (toDebug)
      p->toString();
    Frame f = d->decode(*p);
    if (toDebug)
      f.toString();

    if (!f.isFinished()) {
      if (toDebug)
        logdebug("Frame not finished, continuing");
      continue;
    }
    if (toDebug)
      logdebug("Frame Buffer > 0");

    Frame f2 = conv->convert(f);
    if (toDebug)
      logdebug("Frame Converted");

    f2.setPts(f2.getDts());
    if (toDebug)
      f2.toString();
    Packet ret = e->encode(f2);
    //    if (toDebug)
    //      ret.toString();
    if (multipass == 1 && e->getStatistics())
      statistics.append(e->getStatistics());
    if (false && multipass == 0 || multipass == 2) {
      ETERM * pac = buildTermFromPacket(ret);
      terms.push_back(pac);
    }
    //    if (toDebug)
    //      logdebug("Encoded PacketSize" << ret.getSize());
    delete p;

  }
  delete d;
  delete e;
  delete out_format;
  delete in_format;
  delete conv;
  //  delete sink;
  if (multipass == 1) {
    if (toDebug)
      logdebug("Statistics :" << statistics);
    ETERM * stats = erl_mk_string(statistics.c_str());
    terms.push_back(stats);
  }
  return sink.getTerm();
}




int current_size = 0;

byte * get_buffer(byte * buffer, int size) {
  if (current_size < size) {
    //    logdebug("Increasing Buffer from:" << current_size << " to:" << size);
    if (buffer != NULL)
      delete[]buffer;
    buffer = new byte[size];
    memset(buffer, 0, size);
    current_size = size;
  }
  return buffer;
}

void daemon() {
#ifdef WIN32
  /* Attention Windows programmers: you need to explicitly set
   * mode of stdin/stdout to binary or else the port program won't work
   */
  setmode(fileno(stdout), O_BINARY);
  setmode(fileno(stdin), O_BINARY);
#endif
  //  erl_printf(stderr, "mhive start");
  ETERM *intuple = NULL, *outtuple = NULL;

  byte *buf = get_buffer(NULL, 5000000);

  while (read_cmd(buf) > 0) {
    intuple = erl_decode(buf);
    //        std::cerr<<"InTermSize:"<<erl_size(intuple)<<std::endl;
    if (DEBUG)
      erl_print_term((FILE*) stderr, intuple);
    ETERM* fnp = erl_element(1, intuple);
    if (fnp != NULL) {
      std::string func = (const char*) ERL_ATOM_PTR(fnp);

      if (func == "formatlist") { /*Information Functions for FFMpeg*/
        outtuple = formatlist(intuple);
      } else if (func == "codeclist") {
        outtuple = codeclist(intuple);
      } else if (func == "createfile") {
        outtuple = createfile(intuple);
      } else if (func == "addstream") { /*System functions for creation output Files*/
        outtuple = addstream(intuple);
      } else if (func == "writepacket") {
        outtuple = writepacket(erl_element(2, intuple));
      } else if (func == "writepacketlist") {
        outtuple = writepacketlist(intuple);
      } else if (func == "initfile") {
        outtuple = initfile(intuple);
      } else if (func == "closefile") {
        outtuple = closefile(intuple);
      } else if (func == "fileinfo") { /*System function to get Data from Input Files*/
        outtuple = fileinfo(intuple);
      } else if (func == "streaminfo") {
        outtuple = streaminfo(intuple);
      } else if (func == "packetstream") {
        outtuple = packetstream(intuple);
      } else if (func == "encode") { /*System function to Encode Frames*/
        outtuple = encode(intuple);
      } else {
        std::vector<ETERM *> terms;
        terms.push_back(erl_mk_atom("unknown_command"));
        logdebug("unknown_command");
        outtuple = vector2term(terms);
      }
      if (DEBUG)
        logdebug("try freeing intuple");
      if (intuple != NULL) {
        erl_free_compound(intuple);
        //        erl_free(intuple);
        intuple = NULL;
        if (DEBUG)
          logdebug("intuple freed");
      }
      if (DEBUG)
        logdebug("try return data");
      if (outtuple != NULL) {
        //
        if (DEBUG)
          logdebug("return data:");
        if (DEBUG)
          erl_print_term((FILE*) stderr, outtuple);
        int size = erl_term_len(outtuple);
        //        logdebug("term size:"<<size);
        buf = get_buffer(buf, size);
        if (size > current_size) {
          logerror("OutTuple to big:" << size);
        }
        if (erl_encode(outtuple, buf) > 0) {
          if (DEBUG)
            logdebug("try write_cmd");
          write_cmd(buf, size);
          fflush(NULL);
          if (DEBUG)
            logdebug("write_cmd success");
          erl_free_compound(outtuple);
          //          erl_free(outtuple);
          outtuple = NULL;
          if (DEBUG)
            logdebug("return data written");
        } else {
          erl_print_term((FILE*) stderr, outtuple);
          logerror("Error while encoding term:");
        }
      } else {
        logdebug("OutTuple is NULL");
      }
    }
  }
  delete []buf;
  //  erl_eterm_release();
  logdebug("mhivesys Exiting Normal");

}

/*
 * 
 */

int main(int argc, char** argv) {
  erl_init(NULL, 0);
  av_register_all();
  avcodec_init();
  avcodec_register_all();

  ETERM *intuple = NULL, *outtuple = NULL;
  if (argc > 1 && strcmp(argv[1], "-test") == 0) {
    //	  File f(argv[3]);
    //		FormatInputStream * fis=new FormatInputStream(&f);
    //		FormatInputStream fis(&f);
    //		return 0;
    logdebug("TestMode");
    std::vector<ETERM *> terms;
    if (strcmp(argv[2], "fileinfo") == 0) {
      logdebug("Testing fileinfo");
      terms.push_back(erl_mk_string(""));
      terms.push_back(erl_mk_atom(argv[3]));

      intuple = vector2term(terms);
      erl_print_term((FILE*) stderr, intuple);
      outtuple = fileinfo(intuple);
      erl_print_term((FILE*) stderr, outtuple);
    } else
      if (strcmp(argv[2], "streaminfo") == 0) {
      logdebug("Testing streaminfo");
      terms.push_back(erl_mk_string(""));
      terms.push_back(erl_mk_atom(argv[3]));
      terms.push_back(erl_mk_int(atoi(argv[4])));

      intuple = vector2term(terms);
      erl_print_term((FILE*) stderr, intuple);
      outtuple = streaminfo(intuple);
      erl_print_term((FILE*) stderr, outtuple);
    } else
      if (strcmp(argv[2], "encodevideo") == 0) {
      logdebug("Testing encode");
      ETERM * dec = erl_format("{stream,1,1,0,0,2,undefined,15000000,25,1,90000,720,576,0,12,0,535126463,12711600,0}");
      ETERM * enc = erl_format("{stream,3,2,0,undefined,28,undefined,1024,25,1,25,128,96,undefined,30,0,undefined,undefined,4194304}");


      File infile(argv[3]);
//      int stream_id = 2;
      //    File infile("/media/disk/video/big_buck_bunny_1080p_surround.avi");int stream_id = 1;
//      File outfile("/media/out/test.mp3");

      FormatInputStream fis(&infile);
      PacketInputStream pis(&fis);

//      FormatOutputStream fos(&outfile);
//      PacketOutputStream pos(&fos);

      std::vector<ETERM *> packets;
      Packet p;
      for (int a = 0; a < 100; a++) {
        pis.readPacket(p);
        if (p.getStreamIndex() == 0) {
          packets.push_back(buildTermFromPacket(p));
        }
      }
      ETERM * data=erl_format("{encode,{~s,0,0,~w,~w,~w}}", argv[3], dec, enc, vector2list(packets));
      packets.clear();
//      ETERM * data;

      erl_print_term((FILE*) stderr, data);
      outtuple = encode(data);
      erl_print_term((FILE*) stderr, outtuple);

      erl_free(dec);
      erl_free(enc);
      erl_free(data);
      erl_free_compound(outtuple);
      erl_eterm_release();
    } else
      if (strcmp(argv[2], "encodeaudio") == 0) {
      logdebug("Testing encode");
      ETERM * dec = erl_format("{stream,2,1,1,1,86016,undefined,192000,48000,1,90000,0,0,2,12,1,535188686,12739680,0}");
      ETERM * enc = erl_format("{stream,4,2,1,undefined,86017,undefined,128,44100,1,90000,undefined,undefined,2,20,1,undefined,undefined,4194304}");


      File infile(argv[3]);
//      int stream_id = 2;
      //    File infile("/media/disk/video/big_buck_bunny_1080p_surround.avi");int stream_id = 1;
//      File outfile("/media/out/test.mp3");

      FormatInputStream fis(&infile);
      PacketInputStream pis(&fis);

//      FormatOutputStream fos(&outfile);
//      PacketOutputStream pos(&fos);

      std::vector<ETERM *> packets;
      Packet p;
      for (int a = 0; a < 100; a++) {
        pis.readPacket(p);
        if (p.getStreamIndex() == 1) {
          packets.push_back(buildTermFromPacket(p));
        }
      }
      ETERM * data=erl_format("{encode,{~s,0,0,~w,~w,~w}}", argv[3], dec, enc, vector2list(packets));
      packets.clear();
//      ETERM * data;

      erl_print_term((FILE*) stderr, data);
      outtuple = encode(data);
      erl_print_term((FILE*) stderr, outtuple);

      erl_free(dec);
      erl_free(enc);
      erl_free(data);
      erl_free_compound(outtuple);
      erl_eterm_release();
    }

  } else {
    daemon();
  }
}


