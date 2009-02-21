#include "erl.cpp"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
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
  logdebug("WritePacket");
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
  logdebug("WritePacketList");

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

int main() {
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

  av_register_all();
  avcodec_init();
  avcodec_register_all();
  av_log_level = AV_LOG_ERROR;
  while (read_cmd(buf) > 0) {
    intuple = erl_decode(buf);
    ETERM* fnp = erl_element(1, intuple);
    if (fnp != NULL) {
      std::string func = (const char*) ERL_ATOM_PTR(fnp);
      if (func == "createfile") {
        outtuple = createfile(intuple);
      } else
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
      } else {
        std::vector<ETERM *> terms;
        terms.push_back(erl_mk_atom("unknown_command"));
        outtuple = vector2term(terms);
      }
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