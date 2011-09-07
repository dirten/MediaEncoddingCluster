/* 
 * File:   PresetReaderJson.cpp
 * Author: HoelscJ
 * 
 * Created on 23. Mai 2011, 16:24
 */

#include "PresetReaderJson.h"
#include "org/esb/util/Log.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/av/AV.h"
namespace org {
  namespace esb {
    namespace plugin {

      PresetReaderJson::PresetReaderJson(std::string data) {
        LOGDEBUG("Preset Data:"<<data);
        JSONNode node=libjson::parse(data);

        _preset["id"]=node["format"]["id"].as_string();
        std::string type;
        type="video";
        int c=node[type].size();
        for(int a=0;a<c;a++){
          JSONNode n=node[type].at(a);
          std::string name=n.name();
          /*filter out some unwanted attributes*/
          if(name=="frame_size")
            continue;

          std::string value=n.as_string();
          if(name=="id"){
            name="codec_id";
          }
          if(name=="b"||name=="maxrate"||name=="bt"||name=="bufsize"||name=="minrate")
            value+="000";
          LOGDEBUG("Name="<<name<<" val="<<value);

          _codecs[type].insert(std::pair<std::string, std::string>(name,value));
        }
        
        type="audio";
        c=node[type].size();
        for(int a=0;a<c;a++){
          JSONNode n=node[type].at(a);
          LOGDEBUG("Name="<<n.name()<<" val="<<n.as_string());
          if(n.name()=="id"){
            n.set_name("codec_id");
            //AVCodec * codec=avcodec_find_encoder_by_name(n.as_string().c_str());
            //n=JSONNode("codec_id",codec->id);
          }
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
