/* 
 * File:   PresetReader.cpp
 * Author: jholscher
 * 
 * Created on 25. Oktober 2010, 23:14
 */

#include "PresetReader.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/av/AV.h"
#include "org/esb/av/FormatBaseStream.h"
#include "org/esb/util/StringUtil.h"
namespace org {
  namespace esb {
    namespace hive {
      using namespace rapidxml;

      PresetReader::PresetReader(std::string filename) {
        org::esb::io::File file(filename);
        if (!file.exists()) {
          LOGERROR("Preset File does not exist!");
          return;
        }
        org::esb::av::FormatBaseStream::initialize();
        org::esb::io::FileInputStream fis(filename);
        std::string data;
        fis.read(data);
        rapidxml::xml_document<> _doc;
        try {
          _doc.parse < 0 > (const_cast<char*> (data.c_str()));
        } catch (rapidxml::parse_error ex) {
          LOGERROR("Parser Exception :" << ex.what());
          LOGERROR("Parser Exception :" << ex.where<char>());
          return;
        }
        xml_node<>*preset = _doc.first_node("preset");
        if (!preset) {
          LOGERROR("first element must be a preset");
          return;
        }
        if (!preset->first_attribute("name")) {
          LOGERROR("preset have no name attribute");
          return;
        }
        _preset["name"] = preset->first_attribute("name")->value();
        xml_node<>*format = preset->first_node("format");
        if(!format->first_attribute("id")){
          LOGERROR("no attribute id in element format found");
          return;
        }
        _preset["id"] = format->first_attribute("id")->value();
        for (xml_node<>*param = format->first_node("param"); param; param = param->next_sibling("param")) {
          if (!param->first_attribute("name")) {
            LOGDEBUG("could not resolve name for the preset parameter");
            continue;
          }
          std::string name = param->first_attribute("name")->value();
          if (!param->first_attribute("value")) {
            LOGDEBUG("could not resolve value for the preset parameter name : " << name);
            continue;
          }
          std::string value = param->first_attribute("value")->value();
          LOGDEBUG(" name=" << name << " value=" << value);
          _preset[name] = value;
        }



        handleFormat(preset->first_node("format"));
        for (xml_node<>*codec = preset->first_node("codec"); codec; codec = codec->next_sibling("codec")) {
          handleCodec(codec);
        }
        xml_node<>*filters = preset->first_node("filters");
        for (xml_node<>*filter = filters->first_node("filter"); filter; filter = filter->next_sibling("filter")) {
          handleFilter(filter);
        }
      }

      void PresetReader::handleFormat(rapidxml::xml_node<> *) {
      }

      void PresetReader::handleCodec(rapidxml::xml_node<> *codec) {
        if (!codec->first_attribute("type")) {
          LOGDEBUG("could not resolve type for the codec entry");
          return;
        }
        std::string type = codec->first_attribute("type")->value();
        if (_codecs.count(type) > 0) {
          LOGWARN("multiple codes type " << type << " defined in the preset");
          LOGWARN("it could be only one codec of one type available in the preset");
        }
        if (!codec->first_attribute("id")) {
          LOGERROR("no codec id found for "<<type<<" codec");
        }else{
          std::string id = codec->first_attribute("id")->value();
          AVCodec * codec=avcodec_find_encoder_by_name(id.c_str());
          if(codec){
            std::string t=type+"_codec_id";
            _codecs[type].insert(std::pair<std::string, std::string>(t,org::esb::util::StringUtil::toString(codec->id)));
            _codecs[type].insert(std::pair<std::string, std::string>("codec_id",org::esb::util::StringUtil::toString(codec->id)));
            
          }else{
            LOGWARN("could not find encoder for name "<<id);
          }
        }

        for (xml_node<>*param = codec->first_node("param"); param; param = param->next_sibling("param")) {
          if (!param->first_attribute("name")) {
            LOGDEBUG("could not resolve name for the codec parameter");
            continue;
          }
          std::string name = param->first_attribute("name")->value();
          if (!param->first_attribute("value")) {
            LOGDEBUG("could not resolve value for the codec parameter name : " << name);
            continue;
          }
          std::string value = param->first_attribute("value")->value();
          LOGDEBUG("type=" << type << " name=" << name << " value=" << value);
          _codecs[type].insert(std::pair<std::string, std::string>(name, value));
        }
      }

      void PresetReader::handleFilter(rapidxml::xml_node<> *filter) {
        if (!filter->first_attribute("id")) {
          LOGDEBUG("could not resolve name for the filter entry");
          return;
        }
        std::string id = filter->first_attribute("id")->value();
        if (_filters.count(id) > 0) {
          LOGWARN("multiple filter with the same id " << id << " defined in the preset");
          LOGWARN("it could be only one filter of one id available in the preset");
        }
        for (xml_node<>*param = filter->first_node("param"); param; param = param->next_sibling("param")) {
          if (!param->first_attribute("name")) {
            LOGDEBUG("could not resolve name for the filter parameter");
            continue;
          }
          std::string name = param->first_attribute("name")->value();
          if (!param->first_attribute("value")) {
            LOGDEBUG("could not resolve value for the filter parameter name : " << name);
            continue;
          }
          std::string value = param->first_attribute("value")->value();
          LOGDEBUG("filter id=" << id << " name=" << name << " value=" << value);
          _filters[id].insert(std::pair<std::string, std::string>(name,value));
          /*this is a special case for the codec to know the size of the encode frames*/
          if(id=="resize"){
            if(name=="width"||name=="height")
              _codecs["video"].insert(std::pair<std::string, std::string>(name,value));
          }
        }

      }

      PresetReader::~PresetReader() {
      }

      std::string PresetReader::toString() {
        std::string result;
        result += "\nPreset:\n";
        result += "  name=" + _preset["name"] + "\n";
        if (_preset.size() > 0) {
          result += "FormatParameter:\n";
          std::map<std::string, std::string>::iterator it = _preset.begin();
          for (; it != _preset.end(); it++) {
            result += "  " + (*it).first + "=" + (*it).second + "\n";
          }
        }
        if (_codecs.count("video") > 0) {
          result += "VideoCodec:\n";
          std::multimap<std::string, std::string>::iterator it = _codecs["video"].begin();
          for (; it != _codecs["video"].end(); it++) {
            result += "  " + (*it).first + "=" + (*it).second + "\n";
          }
        }
        if (_codecs.count("audio") > 0) {
          result += "AudioCodec:\n";
          std::multimap<std::string, std::string>::iterator it = _codecs["audio"].begin();
          for (; it != _codecs["audio"].end(); it++) {
            result += "  " + (*it).first + "=" + (*it).second + "\n";
          }
        }
        if (_filters.size() > 0) {
          result += "Filter:\n";
          std::map<std::string, std::multimap<std::string, std::string> >::iterator it = _filters.begin();
          for (; it != _filters.end(); it++) {
            result += "  " + (*it).first + "\n";
            std::multimap<std::string, std::string>::iterator params = (*it).second.begin();
            for (; params != (*it).second.end(); params++) {
              result += "    " + (*params).first + "=" + (*params).second + "\n";
            }
          }
        }
        return result;
      }

      PresetReader::Preset PresetReader::getPreset(){
        return _preset;
      }

      PresetReader::CodecList PresetReader::getCodecList(){
        return _codecs;
      }

      PresetReader::FilterList PresetReader::getFilterList(){
        return _filters;
      }
    }
  }
}
