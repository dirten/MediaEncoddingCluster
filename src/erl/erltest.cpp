
#include "erl.cpp"

#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"

#include "org/esb/util/Log.h"
#include "org/esb/util/Decimal.h"

#include "org/esb/hive/FormatStreamFactory.h"

#include <vector>

using namespace org::esb::av;
using namespace org::esb::util;
using namespace org::esb::hive;
using namespace org::esb::io;

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
    FormatInputStream *fis = FormatStreamFactory::getInputStream(f.getPath());
    if (se >= 0)
      fis->seek(str, se);
    PacketInputStream pis(fis);
    Packet p;
    while (true) {
      pis.readPacket(p);
      if (p.getStreamIndex() == str)break;
    }
    terms.push_back(erl_mk_int(p.getStreamIndex()));
    terms.push_back(erl_mk_int(p.isKeyFrame()));
    terms.push_back(erl_mk_int(p.getPts()));
    terms.push_back(erl_mk_int(p.getDts()));
    terms.push_back(erl_mk_int(p.getSize()));
    terms.push_back(erl_mk_binary((char*) p.getData(), p.getSize()));
  }
  return vector2term(terms);
}

ETERM * packetgroup(ETERM * v) {
  std::vector<ETERM *> terms;
  ETERM * file = erl_element(2, v);
  ETERM * stream = erl_element(3, v);
  ETERM * seek = erl_element(4, v);
  ETERM * packet_count = erl_element(5, v);
  int str = ERL_INT_UVALUE(stream);
  int se = ERL_INT_VALUE(seek);
  int pc = ERL_INT_VALUE(packet_count);
  for (int a = 0; a < pc; a++) {
    terms.push_back(packet(v));
  }
  return vector2term(terms);
}

ETERM * fileinfo(ETERM * v) {
  std::vector<ETERM *> terms;
  ETERM *argp = erl_element(2, v);
  File f((const char*) ERL_ATOM_PTR(argp));
  if (f.exists()) {
//    FormatInputStream fis(&f);
    FormatInputStream *fis = FormatStreamFactory::getInputStream(f.getPath());
    if (!fis->isValid()) {
      terms.push_back(erl_mk_atom("format_invalid"));
    } else {
      terms.push_back(erl_mk_atom(f.getFileName().c_str()));
      terms.push_back(erl_mk_atom(f.getFilePath().c_str()));
      terms.push_back(erl_mk_atom(Decimal(fis->getFileSize()).toString().c_str()));
      terms.push_back(erl_mk_atom(fis->getFormatContext()->iformat->name));
      terms.push_back(erl_mk_int(fis->getStreamCount()));
      terms.push_back(erl_mk_int(fis->getFormatContext()->duration));
      terms.push_back(erl_mk_int(fis->getFormatContext()->bit_rate));
    }
  } else {
    terms.push_back(erl_mk_atom("filenotfound"));
  }
  return vector2term(terms);
}

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

  ETERM *intuple = NULL, *outtuple = NULL;

  byte *buf =new byte[5000000];
  //  memset(&buf,0,sizeof(buf));
  while (read_cmd(buf) > 0) {
    intuple = erl_decode(buf);
    //    std::cerr<<"InTermSize:"<<erl_size(intuple)<<std::endl;
    ETERM* fnp = erl_element(1, intuple);
    if (fnp != NULL) {
      std::string func = (const char*) ERL_ATOM_PTR(fnp);

      if (func == "fileinfo") {
        outtuple = fileinfo(intuple);
      } else if (func == "streaminfo") {
        outtuple = streaminfo(intuple);
      } else if (func == "packet") {
        outtuple = packet(intuple);
      } else if (func == "packetgroup") {
        outtuple = packetgroup(intuple);
      } else {
        std::vector<ETERM *> terms;
        terms.push_back(erl_mk_atom("unknown_command"));
        outtuple = vector2term(terms);
      }
      if(intuple != NULL){
        erl_free_compound(intuple);
        intuple=NULL;
      }

      if (outtuple != NULL) {
//        logdebug("Build Output");
        //        std::cerr<<"InTermSize:"<<erl_size(outtuple)<<std::endl;
        erl_encode(outtuple, buf);
        write_cmd(buf, erl_term_len(outtuple));
        erl_free_compound(outtuple);
        outtuple = NULL;
      }
    }
  }
  delete []buf;



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
