//#include <ei.h>
#include <erl_interface.h>

//#include <unistd.h>
//#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#endif

#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"

#include "org/esb/util/Log.h"
#include "org/esb/util/Decimal.h"

#include "org/esb/hive/FormatStreamFactory.h"

#include <map>
#include <set>
#include <vector>

using namespace org::esb::av;
using namespace org::esb::util;
using namespace org::esb::hive;
using namespace org::esb::io;

int foo(int x) {
  return x + 1;
}

int bar(int y) {
  return y * 2;
}






typedef unsigned char byte;

int read_exact(byte *buf, int len) {
  int i, got = 0;

  do {
    if ((i = read(0, buf + got, len - got)) <= 0)
      return (i);
    got += i;
  } while (got < len);

  return (len);
}

int write_exact(byte *buf, int len) {
  int i, wrote = 0;

  do {
    if ((i = write(1, buf + wrote, len - wrote)) <= 0)
      return (i);
    wrote += i;
  } while (wrote < len);

  return (len);
}

int read_cmd(byte *buf) {
  int len;

  if (read_exact(buf, 4) != 4)
    return (-1);
  len = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
  return read_exact(buf, len);
}

int write_cmd(byte *buf, int len) {
  byte li;

  li = (len >> 24) & 0xff;
  write_exact(&li, 1);

  li = (len >> 16) & 0xff;
  write_exact(&li, 1);

  li = (len >> 8) & 0xff;
  write_exact(&li, 1);

  li = len & 0xff;
  write_exact(&li, 1);

  return write_exact(buf, len);
}

ETERM * vector2term(std::vector<ETERM*> & v) {
  ETERM * term[v.size()];
  std::vector<ETERM*>::iterator it = v.begin();
  for (int a = 0; it != v.end(); it++) {
    term[a++] = *it;
  }
  return erl_mk_tuple(term, v.size());
}

void file_import() {
  byte buf[100000];
  ETERM *intuple, *outtuple, *fnp, *argp, *argp2;
  ETERM * fileinfo[5];
  ETERM * streaminfo[12];
  ETERM * filenotfound[1];

  std::vector<ETERM *> terms;

  logdebug("waiting for Command");
  // return;
  while (read_cmd(buf) > 0) {
    logdebug("Command readed");
    intuple = erl_decode(buf);
    //    std::cout << "Input Size"<<erl_size(intuple)<<std::endl;;
    fnp = erl_element(1, intuple);
    argp = erl_element(2, intuple);
    if (strncmp((const char*) ERL_ATOM_PTR(fnp), "fileinfo", 8) == 0) {
      logdebug("FileInfo on:" << (const char*) ERL_ATOM_PTR(argp));
      File f((const char*) ERL_ATOM_PTR(argp));
      if (f.exists()) {
        FormatInputStream fis(&f);
        terms.push_back(erl_mk_atom(f.getFileName().c_str()));
        terms.push_back(erl_mk_atom(f.getFilePath().c_str()));
        terms.push_back(erl_mk_atom(Decimal(fis.getFileSize()).toString().c_str()));
        terms.push_back(erl_mk_atom(fis.getFormatContext()->iformat->name));
        terms.push_back(erl_mk_int(fis.getStreamCount()));
      } else {
        terms.push_back(erl_mk_atom("filenotfound"));
      }
      outtuple = vector2term(terms); // erl_mk_tuple(filenotfound, 1);
      std::vector<ETERM*>::iterator it = terms.begin();
      for (int a = 0; it != terms.end(); it++) {
        erl_free_term(*it);
        *it = NULL;
      }

      erl_encode(outtuple, buf);
      terms.clear();
      write_cmd(buf, erl_term_len(outtuple));

    } else if (strncmp((const char*) ERL_ATOM_PTR(fnp), "streaminfo", 10) == 0) {
      //-record(stream,{id,fileid,streamidx,streamtype,codec,rate,num, den, width, height,channels,gop,format}).
      argp2 = erl_element(3, intuple);
      int s = ERL_INT_UVALUE(argp2);
      File f((const char*) ERL_ATOM_PTR(argp));
      if (f.exists()) {
        FormatInputStream fis(&f);
        AVStream *str = fis.getFormatContext()->streams[s];
        if (s < 0 || str == NULL) {
          filenotfound[0] = erl_mk_atom("streamnotfound");
          outtuple = erl_mk_tuple(filenotfound, 1);
        } else {
          streaminfo[0] = erl_mk_int(0);
          streaminfo[1] = erl_mk_int(str->index);
          streaminfo[2] = erl_mk_int(str->codec->codec_type);
          streaminfo[3] = erl_mk_int(str->codec->codec_id);
          streaminfo[4] = erl_mk_int(str->codec->codec_type == CODEC_TYPE_VIDEO ? av_q2d(str->r_frame_rate) : str->codec->sample_rate);
          streaminfo[5] = erl_mk_int(str->time_base.num);
          streaminfo[6] = erl_mk_int(str->time_base.den);
          streaminfo[7] = erl_mk_int(str->codec->width);
          streaminfo[8] = erl_mk_int(str->codec->height);
          streaminfo[9] = erl_mk_int(str->codec->channels);
          streaminfo[10] = erl_mk_int(str->codec->gop_size);
          streaminfo[11] = erl_mk_int(str->codec->codec_type == CODEC_TYPE_VIDEO ? str->codec->pix_fmt : str->codec->sample_fmt);
          outtuple = erl_mk_tuple(streaminfo, 12);
        }
      } else {
        filenotfound[0] = erl_mk_atom("filenotfound");
        outtuple = erl_mk_tuple(filenotfound, 1);
      }
      erl_encode(outtuple, buf);
      write_cmd(buf, erl_term_len(outtuple));
    } else if (strncmp((const char*) ERL_ATOM_PTR(fnp), "packets", 7) == 0) {

    }
  }
}

ETERM * streaminfo(ETERM * v) {
  std::vector<ETERM *> terms;
  ETERM *file = erl_element(2, v);
  ETERM *stream = erl_element(3, v);
  int s = ERL_INT_UVALUE(stream);
  File f((const char*) ERL_ATOM_PTR(file));
  if (f.exists()) {
    FormatInputStream fis(&f);
    if (!fis.isValid() || s < 0 || s >= fis.getFormatContext()->nb_streams) {
      terms.push_back(erl_mk_atom("streamnotfound"));
    } else {
      AVStream *str = fis.getFormatContext()->streams[s];
      terms.push_back(erl_mk_int(0));
      terms.push_back(erl_mk_int(str->index));
      terms.push_back(erl_mk_int(str->codec->codec_type));
      terms.push_back(erl_mk_int(str->codec->codec_id));
      terms.push_back(erl_mk_int(str->codec->codec_type == CODEC_TYPE_VIDEO ? av_q2d(str->r_frame_rate) : str->codec->sample_rate));
      terms.push_back(erl_mk_int(str->time_base.num));
      terms.push_back(erl_mk_int(str->time_base.den));
      terms.push_back(erl_mk_int(str->codec->width));
      terms.push_back(erl_mk_int(str->codec->height));
      terms.push_back(erl_mk_int(str->codec->channels));
      terms.push_back(erl_mk_int(str->codec->gop_size));
      terms.push_back(erl_mk_int(str->codec->codec_type == CODEC_TYPE_VIDEO ? str->codec->pix_fmt : str->codec->sample_fmt));
    }
  } else {
    terms.push_back(erl_mk_atom("filenotfound"));
  }
  return vector2term(terms);

}
ETERM * packet(ETERM * v) {
  std::vector<ETERM *> terms;
  ETERM *file = erl_element(2, v);
  ETERM *stream = erl_element(3, v);
  ETERM *seek = erl_element(4, v);
  int str = ERL_INT_UVALUE(stream);
  int se = ERL_INT_VALUE(seek);

  File f((const char*) ERL_ATOM_PTR(file));
  if (f.exists()) {
    FormatInputStream *fis=FormatStreamFactory::getInputStream(f.getPath());
    if(se>=0)
        fis->seek(str,se);
    PacketInputStream pis(fis);
    Packet p;
    pis.readPacket(p);
    terms.push_back(erl_mk_int(p.getStreamIndex()));
    terms.push_back(erl_mk_int(p.isKeyFrame()));
    terms.push_back(erl_mk_int(p.getPts()));
    terms.push_back(erl_mk_int(p.getDts()));
    terms.push_back(erl_mk_int(p.getSize()));
    terms.push_back(erl_mk_binary((char*)p.getData(),p.getSize()));
  }
  return vector2term(terms);
}

ETERM * fileinfo(ETERM * v) {
  std::vector<ETERM *> terms;
  ETERM *argp = erl_element(2, v);
  File f((const char*) ERL_ATOM_PTR(argp));
  if (f.exists()) {
    FormatInputStream fis(&f);
    terms.push_back(erl_mk_atom(f.getFileName().c_str()));
    terms.push_back(erl_mk_atom(f.getFilePath().c_str()));
    terms.push_back(erl_mk_atom(Decimal(fis.getFileSize()).toString().c_str()));
    terms.push_back(erl_mk_atom(fis.getFormatContext()->iformat->name));
    terms.push_back(erl_mk_int(fis.getStreamCount()));
  } else {
    terms.push_back(erl_mk_atom("filenotfound"));
  }
  return vector2term(terms);
}

int main() {
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

  ETERM *intuple = NULL, *outtuple = NULL;

  byte buf[100000];

  while (read_cmd(buf) > 0) {
    intuple = erl_decode(buf);
    ETERM* fnp = erl_element(1, intuple);
    if (fnp != NULL) {
      std::string func = (const char*) ERL_ATOM_PTR(fnp);
      if (func == "fileinfo") {
        outtuple = fileinfo(intuple);
      } else if (func == "streaminfo") {
        outtuple = streaminfo(intuple);
      } else if (func == "packet") {
        outtuple = packet(intuple);
      } else {
        std::vector<ETERM *> terms;
        terms.push_back(erl_mk_atom("unknown_command"));
        outtuple = vector2term(terms);
      }

      if (outtuple != NULL) {
        erl_encode(outtuple, buf);
        write_cmd(buf, erl_term_len(outtuple));
        erl_free_compound(outtuple);
        outtuple = NULL;
      }
    }
  }




  /*
    erl_free_compound(tuplep);
    erl_free_compound(tuple);
    //    erl_free_compound(arr);
    erl_free_term(arr[0]);
    erl_free_term(arr[1]);
    erl_free_term(arr[2]);
    //    erl_free_term(arr[0]);
    erl_free_term(fnp);
    erl_free_term(argp);
   */
  //    erl_free_term(intp);

  //  logdebug("Program End");

}
