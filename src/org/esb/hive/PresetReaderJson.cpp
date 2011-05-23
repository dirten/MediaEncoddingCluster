/* 
 * File:   PresetReaderJson.cpp
 * Author: HoelscJ
 * 
 * Created on 23. Mai 2011, 16:24
 */

#include "PresetReaderJson.h"
#include "org/esb/util/Log.h"
#include "org/esb/libjson/libjson.h"
namespace org {
  namespace esb {
    namespace hive {

      PresetReaderJson::PresetReaderJson(std::string data) {
        LOGDEBUG("Preset Data:"<<data);
        JSONNode node=libjson::parse(data);
        _preset["id"]=node["format"]["id"].as_string();
        std::string type;
        type="video";
        int c=node[type].size();
        for(int a=0;a<c;a++){
          JSONNode n=node[type].at(a);
          LOGDEBUG("Name="<<n.name()<<" val="<<n.as_string());
          if(n.name()=="id")
            n.set_name("codec_id");
          _codecs[type].insert(std::pair<std::string, std::string>(n.name(),n.as_string()));
        }
        
        type="audio";
        c=node[type].size();
        for(int a=0;a<c;a++){
          JSONNode n=node[type].at(a);
          LOGDEBUG("Name="<<n.name()<<" val="<<n.as_string());
          if(n.name()=="id")
            n.set_name("codec_id");
          _codecs[type].insert(std::pair<std::string, std::string>(n.name(),n.as_string()));
        }
      }

      PresetReaderJson::~PresetReaderJson() {
      }

      PresetReaderJson::Preset PresetReaderJson::getPreset() {
        return _preset;
      }

      PresetReaderJson::CodecList PresetReaderJson::getCodecList() {
        return _codecs;
      }

      PresetReaderJson::FilterList PresetReaderJson::getFilterList() {
        return _filters;
      }

    }
  }
}
