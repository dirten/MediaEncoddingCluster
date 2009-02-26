/* 
 * File:   erlsys.cpp
 * Author: jhoelscher
 *
 * Created on 26. Februar 2009, 13:23
 */

//#include <stdlib.h>
#include "erl.cpp"
#include "org/esb/av/AV.h"

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

      if (func == "formatlist") {
        outtuple = formatlist(intuple);
      } else
        if (func == "codeclist") {
        outtuple = codeclist(intuple);
      } else {
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

