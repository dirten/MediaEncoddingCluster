/* 
 * File:   erltest.cpp
 * Author: jhoelscher
 *
 * Created on 15. Juni 2009, 18:31
 */

#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameConverter.h"
#  include "org/esb/util/Decimal.h"


#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/FileOutputStream.h"


using namespace org::esb::io;
using namespace org::esb::av;
using namespace org::esb::util;


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "org/esb/util/Log.h"
#include "org/esb/lang/Thread.h"
extern "C" {
#include <erl_interface.h>
}
typedef unsigned char byte;

int read_exact(byte *buf, int len, FILE*d) {
  int i, got = 0;

  do {
    if ((i = fread(buf + got, 1, len - got, d)) <= 0)
      return (i);
    got += i;
  } while (got < len);
  //  logdebug("Bytes readed" << len << ":Buffer" << buf);
  return (len);
}

int write_exact(byte *buf, int len, FILE*d) {
  int i, wrote = 0;

  do {
    if ((i = fwrite(buf + wrote, 1, len - wrote, d)) <= 0)
      return (i);
    wrote += i;
  } while (wrote < len);

  return (len);
}

int read_cmd(byte *buf, FILE*d) {
  //  logdebug("Wating for read");
  int len;

  if (read_exact(buf, 4, d) != 4)
    return (-1);
  len = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
  //  logdebug("read length:" << len);
  return read_exact(buf, len, d);
}

int write_cmd(byte *buf, int len, FILE*d) {
  //  logdebug("Try Write Command:"<<buf);
  byte li;

  li = (len >> 24) & 0xff;
  write_exact(&li, 1, d);

  li = (len >> 16) & 0xff;
  write_exact(&li, 1, d);

  li = (len >> 8) & 0xff;
  write_exact(&li, 1, d);

  li = len & 0xff;
  write_exact(&li, 1, d);
  //  logdebug("Write Buffer with Length "<<len);
  return write_exact(buf, len, d);
}
ETERM * vector2list(std::vector<ETERM*> & v) {
  const int s = static_cast<const int> (v.size());
  ETERM ** term = new ETERM*[s];
  std::vector<ETERM*>::iterator it = v.begin();
/*
  for (int a = 0; a< v.size(); a++) {
    logdebug("vector to list first");
    erl_print_term((FILE*) stderr, v[a]);
  }*/
  for (int a = 0; it != v.end(); it++) {
//    logdebug("vector to list");
//    erl_print_term((FILE*) stderr, *it);
    term[a++] = *it;
  }
  return erl_mk_list(term, v.size());
}

ETERM * vector2tuple(std::vector<ETERM*> & v) {
  const int s = static_cast<const int> (v.size());
  ETERM ** term = new ETERM*[(const int) s];
  std::vector<ETERM*>::iterator it = v.begin();
  for (int a = 0; it != v.end(); it++) {
    term[a++] = *it;
  }
  return erl_mk_tuple(term, v.size());
}

ETERM * buildTermFromPacket(Packet & p) {
  std::vector<ETERM *> terms;
  terms.push_back(erl_mk_int(p.getStreamIndex()));
  terms.push_back(erl_mk_int(p.isKeyFrame()));
  //    terms.push_back(erl_mk_string(toString(p.getPts()).c_str()));
  //    terms.push_back(erl_mk_string(toString(p.getDts()).c_str()));
  terms.push_back(erl_mk_string(Decimal(p.getPts()).toString().c_str()));
  terms.push_back(erl_mk_string(Decimal(p.getDts()).toString().c_str()));
  //    terms.push_back(erl_mk_binary((const char *)p.getPts(),8));
  //    terms.push_back(erl_mk_binary((const char*)p.getDts(),8));
  terms.push_back(erl_mk_int(p.getFlags()));
  terms.push_back(erl_mk_int(p.getDuration()));
  terms.push_back(erl_mk_int(p.getSize()));
  if(p.getSize()>0){
//    logdebug("Setting binary");
    terms.push_back(erl_mk_binary((char*) p.getData(), p.getSize()));
  }
//  logdebug("make vector2term from packet");
  return vector2tuple(terms);
}

/*
 * little testprogramm for testing the mhive-ffmpeg api over erlang
 */
int main(int argc, char** argv) {
  pid_t pid;

  int p2c[2], c2p[2];

  pipe(p2c);
  pipe(c2p);


  pid = fork();
  if (pid == 0) {
    close(0);
    dup(p2c[0]);
    close(1);
    close(p2c[1]);

    dup(c2p[1]);
    close(c2p[0]);
    //    std::cout << "start programm" << std::endl;
    int s = execlp("../src/erl/bin/mhivesys", argv[0], NULL);
    std::cout << "Status :" << s << std::endl;
  } else {
    org::esb::lang::Thread::sleep2(1000);
    FILE *tochild, *fromchild;
    tochild = fdopen(p2c[1], "w");
    close(p2c[0]);
    fromchild = fdopen(c2p[0], "r");
    close(c2p[1]);


    erl_init(NULL, 0);

    int stream_id = 2;

    File infile("/media/video/ChocolateFactory.ts");
    File outfile("/media/out/test.mp3");

    FormatInputStream fis(&infile);
    PacketInputStream pis(&fis);

    FormatOutputStream fos(&outfile);
    PacketOutputStream pos(&fos);

    AVCodecContext * c = fis.getFormatContext()->streams[stream_id]->codec;
    Decoder dec(c->codec_id);
    dec.setChannels(c->channels);
    dec.setBitRate(c->bit_rate);
    dec.setSampleRate(c->sample_rate);
    dec.setPixelFormat(c->pix_fmt);
    dec.setPixelFormat(PIX_FMT_YUV420P);
    dec.ctx->request_channel_layout = 2;
    dec.open();

    Encoder enc(CODEC_ID_VORBIS);
    enc.setChannels(2);
    enc.setBitRate(448000);
    enc.setSampleRate(48000);
    enc.setSampleFormat(dec.getSampleFormat());

    enc.setTimeBase((AVRational) {
      1, 48000
    });
    enc.setFlag(CODEC_FLAG_GLOBAL_HEADER);

    enc.setPixelFormat(PIX_FMT_YUV420P);
    enc.open();
    

    std::vector<ETERM *> terms;
/*
    //{fileinfo,'/media/video/ChocolateFactory.ts',0,0,0}
    terms.push_back(erl_mk_atom("fileinfo"));
    //    terms.push_back(erl_mk_atom("/media/video/ChocolateFactory.ts"));
    terms.push_back(erl_mk_atom(infile.getPath().c_str()));
    terms.push_back(erl_mk_int(0));
    terms.push_back(erl_mk_int(0));
    terms.push_back(erl_mk_int(0));
 */

    std::vector<ETERM *> info;
    info.push_back(erl_mk_string(infile.getPath().c_str()));
    info.push_back(erl_mk_int(0));
    info.push_back(erl_mk_int(0));
//    {stream,2,1,1,1,86016,undefined,192000,48000,1,90000,0,0,2,12,1,-535188686,12739680,0},
//    {stream,1,1,0,0,2,undefined,15000000,25,1,90000,720,576,0,12,0,-535126463,12711600,0},
    std::vector<ETERM *> decoder;
    decoder.push_back(erl_mk_atom("stream"));
    decoder.push_back(erl_mk_int(2));
    decoder.push_back(erl_mk_int(1));
    decoder.push_back(erl_mk_int(1));
    decoder.push_back(erl_mk_int(1));
    decoder.push_back(erl_mk_int(dec.getCodecId()));
    decoder.push_back(erl_mk_atom("undefined"));
    decoder.push_back(erl_mk_int(192000));
    decoder.push_back(erl_mk_int(48000));
    decoder.push_back(erl_mk_int(1));
    decoder.push_back(erl_mk_int(90000));
    decoder.push_back(erl_mk_int(0));
    decoder.push_back(erl_mk_int(0));
    decoder.push_back(erl_mk_int(2));
    decoder.push_back(erl_mk_int(12));
    decoder.push_back(erl_mk_int(1));
    decoder.push_back(erl_mk_int(-535126463));
    decoder.push_back(erl_mk_int(12711600));
    decoder.push_back(erl_mk_int(0));


//  {stream,4,2,1,undefined,86017,undefined,192000,48000,1,90000,undefined,undefined,2,20,1,undefined,undefined,0}
//  {stream,3,2,0,undefined,13,undefined,1024,25,1,25,640,480,undefined,30,0,undefined,undefined,0},
    std::vector<ETERM *> encoder;
    encoder.push_back(erl_mk_atom("stream"));
    encoder.push_back(erl_mk_int(4));
    encoder.push_back(erl_mk_int(2));
    encoder.push_back(erl_mk_int(1));
    encoder.push_back(erl_mk_atom("undefined"));
    encoder.push_back(erl_mk_int(enc.getCodecId()));
    encoder.push_back(erl_mk_atom("undefined"));
    encoder.push_back(erl_mk_int(192));
    encoder.push_back(erl_mk_int(48000));
    encoder.push_back(erl_mk_int(1));
    encoder.push_back(erl_mk_int(90000));
    encoder.push_back(erl_mk_int(0));
    encoder.push_back(erl_mk_int(0));
    encoder.push_back(erl_mk_int(2));
    encoder.push_back(erl_mk_int(20));
    encoder.push_back(erl_mk_int(1));
    encoder.push_back(erl_mk_atom("undefined"));
    encoder.push_back(erl_mk_atom("undefined"));
    encoder.push_back(erl_mk_int(0));

    std::vector<ETERM *> packets;
    Packet p;
    for(int a=0;a<100;a++){
      pis.readPacket(p);
      if(p.getStreamIndex()!=1)continue;
      packets.push_back(buildTermFromPacket(p));
    }

    info.push_back(vector2tuple(decoder));
    info.push_back(vector2tuple(encoder));
    info.push_back(vector2list(packets));
    terms.push_back(erl_mk_atom("encode"));
    terms.push_back(vector2tuple(info));

    ETERM* writing = vector2tuple(terms);


//{encode,{"/media/video/ChocolateFactory.ts",10,0,
//    {stream,1,1,0,0,2,undefined,15000000,25,1,90000,720,576,0,12,0,-535126463,12711600,0},
//    {stream,3,2,0,undefined,13,undefined,1024,25,1,25,640,480,undefined,30,0,undefined,undefined,0},
//        [{0,1,"3767973233","3767962433",1,3600,33508,#Bin},{0,0,"3767966033","3767966033",0,3600,7108,#Bin},{0,0,"3767969633","3767969633",0,3600,6636,#Bin},{0,0,"-9223372036854775808","3767973233",0,3600,17860,#Bin},{0,0,"3767976833","3767976833",0,3600,6588,#Bin},{0,0,"3767980433","3767980433",0,3600,7268,#Bin},{0,0,"-9223372036854775808","3767984033",0,3600,19596,#Bin},{0,0,"3767987633","3767987633",0,3600,7180,#Bin},{0,0,"3767991233","3767991233",0,3600,7156,#Bin},{0,0,"-9223372036854775808","3767994833",0,3600,17860,#Bin},{0,0,"3767998433","3767998433",0,3600,5964,#Bin},{0,0,"3768002033","3768002033",0,3600,5964,#Bin},{0,1,"3768016433","3768005633",1,3600,31180,#Bin},{0,0,"3768009233","3768009233",0,3600,5076,#Bin},{0,0,"3768012833","3768012833",0,3600,5708,#Bin},{0,0,"-9223372036854775808","3768016433",0,3600,15572,#Bin},{0,0,"3768020033","3768020033",0,3600,5268,#Bin},{0,0,"3768023633","3768023633",0,3600,5884,#Bin},{0,0,"-9223372036854775808","3768027233",0,3600,15324,#Bin},{0,0,"3768030833","3768030833",0,3600,5404,#Bin},{0,0,"3768034433","3768034433",0,3600,5628,#Bin},{0,0,"-9223372036854775808","3768038033",0,3600,16508,#Bin},{0,0,"3768041633","3768041633",0,3600,5556,#Bin},{0,0,"3768045233","3768045233",0,3600,6220,#Bin},{0,1,"3768059633","3768048833",1,3600,32284,#Bin},{0,0,"3768052433","3768052433",0,3600,5316,#Bin},{0,0,"3768056033","3768056033",0,3600,5220,#Bin}]
//  }
//}


    int len = erl_term_len(writing);
    int bytes = 0;
    byte * buf = new byte[len];
    byte * inbuf = new byte[100000];
    erl_print_term((FILE*) stderr, writing);

    erl_encode(writing, buf);
    logdebug("writing buffer:" << buf);
    write_cmd(buf, len, tochild);
    fflush(tochild);
    read_cmd(inbuf, fromchild);

    ETERM*intuple = erl_decode(inbuf);
    logdebug("read_cmd()");
    erl_print_term((FILE*) stderr, intuple);
  }
  return (EXIT_SUCCESS);
}

