

#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"

#include "org/esb/util/Log.h"
#include "org/esb/util/Decimal.h"

#include "org/esb/hive/FormatStreamFactory.h"

#include <boost/shared_ptr.hpp>
#include "erl.cpp"

#include <vector>
#include <list>
using namespace org::esb::av;
using namespace org::esb::util;
using namespace org::esb::hive;
using namespace org::esb::io;
std::list<boost::shared_ptr<Packet> > last_packet_list;
std::list<boost::shared_ptr<Packet> > audio_packet_list;
bool packet_group_break=false;
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

ETERM * packet(ETERM * v) {
//  std::vector<ETERM *> terms;
  ETERM *file = erl_element(2, v);
  ETERM *stream = erl_element(3, v);
  ETERM *seek = erl_element(4, v);
  ETERM * packet=NULL;
  int str = ERL_INT_UVALUE(stream);
  int se = ERL_INT_VALUE(seek);

  File f((const char*) ERL_ATOM_PTR(file));
  if (f.exists()) {
    FormatInputStream *fis = FormatStreamFactory::getInputStream(f.getPath());
    if (se >= 0)
      fis->seek(str, se);
    PacketInputStream pis(fis);
    Packet p;
    while (pis.readPacket(p) >= 0) {
        if (p.getStreamIndex() == str)break;
    }
    packet=buildTermFromPacket(p);
/*
    terms.push_back(erl_mk_int(p.getStreamIndex()));
    terms.push_back(erl_mk_int(p.isKeyFrame()));
    terms.push_back(erl_mk_atom(Decimal(p.getPts()).toString().c_str()));
    terms.push_back(erl_mk_atom(Decimal(p.getDts()).toString().c_str()));
    terms.push_back(erl_mk_int(p.getFlags()));
    terms.push_back(erl_mk_int(p.getDuration()));
    terms.push_back(erl_mk_int(p.getSize()));
    terms.push_back(erl_mk_binary((char*) p.getData(), p.getSize()));
 */
    boost::shared_ptr<Packet> pPacket(new Packet(p));

    last_packet_list.push_back(pPacket);
    if (last_packet_list.size() > 10)
      last_packet_list.pop_front();
    
  }
//  return vector2term(terms);
  return packet;
}

ETERM * packetgroup(ETERM * v) {
  std::vector<ETERM *> terms;
  ETERM * packet_count = erl_element(5, v);
  int pc = ERL_INT_VALUE(packet_count);
  packet_group_break=false;
  if (last_packet_list.size() > 0) {
    std::vector<ETERM *> myterm;
    boost::shared_ptr<Packet>p = last_packet_list.back();
    myterm.push_back(erl_mk_int(p->getStreamIndex()));
    myterm.push_back(erl_mk_int(p->isKeyFrame()));
    myterm.push_back(erl_mk_atom(Decimal(p->getPts()).toString().c_str()));
    myterm.push_back(erl_mk_atom(Decimal(p->getDts()).toString().c_str()));
    myterm.push_back(erl_mk_int(p->getFlags()));
    myterm.push_back(erl_mk_int(p->getDuration()));
    myterm.push_back(erl_mk_int(p->getSize()));
    myterm.push_back(erl_mk_binary((char*) p->getData(), p->getSize()));
    terms.push_back(vector2term(myterm));
    last_packet_list.pop_front();
  }
  for (int a = 0; pc < 0 || a < pc; a++) {
    ETERM * p = packet(v);

    if ((pc < 0 && (last_packet_list.back()->isKeyFrame()) || last_packet_list.back()->getSize() <= 0)) {
      break;
    }
    terms.push_back(p);
    if(packet_group_break)break;
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
      terms.push_back(erl_mk_string(f.getFileName().c_str()));
      terms.push_back(erl_mk_string(f.getFilePath().c_str()));
      terms.push_back(erl_mk_string(Decimal(fis->getFileSize()).toString().c_str()));
      terms.push_back(erl_mk_string(fis->getFormatContext()->iformat->name));
      terms.push_back(erl_mk_int(fis->getStreamCount()));
      terms.push_back(erl_mk_string(Decimal(fis->getFormatContext()->duration).toString().c_str()));
      terms.push_back(erl_mk_int(fis->getFormatContext()->bit_rate));
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
  memset(buf,0,5000000);
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
      } else if (func == "packetstream") {
        outtuple = packetstream(intuple);
      } else {
        std::vector<ETERM *> terms;
        terms.push_back(erl_mk_atom("unknown_command"));
        outtuple = vector2term(terms);
      }
      if (intuple != NULL) {
        erl_free_compound(intuple);
        intuple = NULL;
      }

      if (outtuple != NULL) {
        //        logdebug("Build Output");
        //        std::cerr<<"InTermSize:"<<erl_size(outtuple)<<std::endl;
        int size=erl_term_len(outtuple);
        if(size>5000000){
          logerror("OutTuple to big(max 5000000b):"<<size);
        }
        erl_encode(outtuple, buf);
        write_cmd(buf, size);
        erl_free_compound(outtuple);
        outtuple = NULL;
      }
    }
//    erl_free_compound(intuple);
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
