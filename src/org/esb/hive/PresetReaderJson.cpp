/* 
 * File:   PresetReaderJson.cpp
 * Author: HoelscJ
 * 
 * Created on 23. Mai 2011, 16:24
 */
#include "org/esb/db/hivedb.hpp"
#include "PresetReaderJson.h"
#include "org/esb/util/Log.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/av/AV.h"
#include "org/esb/lang/Exception.h"
#include "org/esb/config/config.h"
#include "DatabaseService.h"
namespace org {
  namespace esb {
    namespace hive {

      PresetReaderJson::PresetReaderJson(std::string data) {
        LOGDEBUG("Preset Data:" << data);
        JSONNode node = libjson::parse(data);
        if(node.contains("profile-uuid")){
          db::HiveDb db=DatabaseService::getDatabase();//"sqlite3", org::esb::config::Config::get("db.url"));
          litesql::DataSource<db::Preset>s = litesql::select<db::Preset > (db, db::Preset::Uuid == node["profile-uuid"].as_string());
          if(s.count()==1){
            data=s.one().data;
          }else{
            throw org::esb::lang::Exception(__FILE__,__LINE__,"profile defined by profile-uuid not found");
          }
          node = libjson::parse(data);
        }
        if (node.contains("format") && node["format"].contains("id")) {
          _preset["id"] = node["format"]["id"].as_string();
          if(node.contains("name")){
            _preset["name"]=node["name"].as_string();
          }
        }else{
          throw org::esb::lang::Exception(__FILE__,__LINE__,"no format attribute found");
        }
        std::string type;
        type = "video";
        if (node.contains(type)) {
          int c = node[type].size();
          for (int a = 0; a < c; a++) {
            JSONNode n = node[type].at(a);
            std::string name = n.name();
            /*filter out some unwanted attributes*/
            if (name == "frame_size")
              continue;

            std::string value = n.as_string();
            if (name == "id") {
              name = "codec_id";
            }
            if (name == "b" || name == "maxrate" || name == "bt" || name == "bufsize" || name == "minrate")
              value += "000";
            LOGDEBUG("Name=" << name << " val=" << value);

            _codecs[type][name] = value; //.insert(std::pair<std::string, std::string>(name,value));
          }
        }else{
          throw org::esb::lang::Exception(__FILE__,__LINE__,"no video attribute found");
        }
        type = "audio";
        if (node.contains(type)) {
          int c = node[type].size();
          for (int a = 0; a < c; a++) {
            JSONNode n = node[type].at(a);
            LOGDEBUG("Name=" << n.name() << " val=" << n.as_string());
            if (n.name() == "id") {
              n.set_name("codec_id");
              //AVCodec * codec=avcodec_find_encoder_by_name(n.as_string().c_str());
              //n=JSONNode("codec_id",codec->id);
            }
            _codecs[type][n.name()] = n.as_string(); //.insert(std::pair<std::string, std::string>(n.name(),n.as_string()));
          }
        }else{
          throw org::esb::lang::Exception(__FILE__,__LINE__,"no audio attribute found");
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
