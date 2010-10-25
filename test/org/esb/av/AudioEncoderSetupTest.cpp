/* 
 * File:   AudioEncoderSetupTest.cpp
 * Author: HoelscJ
 *
 * Created on 25. Oktober 2010, 15:49
 */

#include "org/esb/hive/CodecPropertyTransformer.h"
#include <cstdlib>
#include "org/esb/av/Encoder.h"
#include "org/esb/av/FormatBaseStream.h"
#include "org/esb/lang/Ptr.h"
#include "org/esb/util/Log.h"
#include <map>
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  org::esb::av::FormatBaseStream::initialize();
  std::map<std::string, std::string> data;
  data["parti8x8"] = "1";
  data["partb8x8"] = "1";
  data["parti4x4"] = "1";
  data["partp4x4"] = "0";
  data["ac"] = "2";
  data["ar"] = "44100";
  data["g"] = "250";
  data["bt"] = "300000";
  data["b"] = "1024000";
  data["ab"] = "128000";
  data["chroma"] = "1";
  data["mixed_refs"] = "1";
  data["dct8x8"] = "1";
  data["blafasel"] = "test unknown";
  org::esb::hive::CodecPropertyTransformer * trans = new org::esb::hive::CodecPropertyTransformer(data);


  Ptr<org::esb::av::Encoder> e = new org::esb::av::Encoder(CODEC_ID_MP2);
  std::map<std::string, std::string> params = trans->getCodecProperties();
  std::map<std::string, std::string>::iterator it = params.begin();
  for (; it != params.end(); it++) {
    if ((*it).second.length() > 0) {
      if (e->setCodecOption((*it).first, (*it).second)) {
        LOGERROR("setting CodecOptionsPair (opt=" << (*it).first << " arg=" << (*it).second << ")");
      }
    }
  }
  e->open();
  assert(e->getBitRate()==128000);
  assert(e->getChannels()==2);
  assert(e->getSampleRate()==44100);
  return 0;
}

