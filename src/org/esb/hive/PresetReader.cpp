/* 
 * File:   PresetReader.cpp
 * Author: jholscher
 * 
 * Created on 25. Oktober 2010, 23:14
 */

#include "PresetReader.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/util/Log.h"
#include "boost/property_tree/detail/rapidxml.hpp"
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
        org::esb::io::FileInputStream fis(filename);
        std::string data;
        fis.read(data);
        rapidxml::xml_document<> _doc;
        try {
          _doc.parse < 0 > (const_cast<char*> (data.c_str()));
        } catch (rapidxml::parse_error ex) {
          LOGERROR("Parser Exception :" << ex.what());
          return;
        }
        xml_node<>*preset = _doc.first_node("preset");
        if(!preset){
          LOGERROR("first element must be a preset");
          return;
        }
        if(!preset->first_attribute("name")){
          LOGERROR("preset have no name attribute");
          return;
        }
        _preset["name"]=preset->first_attribute("name")->value();
        xml_node<>*format = preset->first_node("format");
        for (xml_node<>*param = format->first_node("param"); param; param = param->next_sibling("param")) {
          if(!param->first_attribute("name")){
            LOGDEBUG("could not resolve name for the preset parameter");
            continue;
          }
          std::string name=param->first_attribute("name")->value();
          if(!param->first_attribute("value")){
            LOGDEBUG("could not resolve value for the preset parameter name : "<<name);
            continue;
          }
          std::string value=param->first_attribute("value")->value();
          LOGDEBUG(" name="<<name<<" value="<<value);
          _preset[name]=value;
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
        if(!codec->first_attribute("type")){
          LOGDEBUG("could not resolve type for the codec entry");
          return;
        }
        std::string type = codec->first_attribute("type")->value();
        if(_codecs.count(type)>0){
          LOGWARN("multiple codes type "<<type<<" defined in the preset");
          LOGWARN("it could be only one codec of one type available in the preset");
        }
        for (xml_node<>*param = codec->first_node("param"); param; param = param->next_sibling("param")) {
          if(!param->first_attribute("name")){
            LOGDEBUG("could not resolve name for the codec parameter");
            continue;
          }
          std::string name=param->first_attribute("name")->value();
          if(!param->first_attribute("value")){
            LOGDEBUG("could not resolve value for the codec parameter name : "<<name);
            continue;
          }
          std::string value=param->first_attribute("value")->value();
          LOGDEBUG("type="<<type<<" name="<<name<<" value="<<value);
          _codecs[type][name]=value;
        }
      }

      void PresetReader::handleVideoCodec(rapidxml::xml_node<> *codec) {
        for (xml_node<>*param = codec->first_node("param"); param; param = param->next_sibling("param")) {
          if(!param->first_attribute("name")){
            LOGDEBUG("could not resolve name for the codec parameter");
            continue;
          }
          std::string name=param->first_attribute("name")->value();
          if(!param->first_attribute("value")){
            LOGDEBUG("could not resolve value for the codec parameter name : "<<name);
            continue;
          }
          std::string value=param->first_attribute("name")->value();
          _codecs["video"][name]=value;
        }
      }

      void PresetReader::handleAudioCodec(rapidxml::xml_node<> *) {
      }

      void PresetReader::handleFilterList(rapidxml::xml_node<> *) {
      }

      void PresetReader::handleFilter(rapidxml::xml_node<> *) {
      }

      PresetReader::~PresetReader() {
      }

      void PresetReader::loadIntoDB() {
      }

      std::string PresetReader::toString() {
        std::string result;
        result+="\nPreset:\n";
        result+="\tname="+_preset["name"]+"\n";
        if(_preset.size()>0){
          result+="FormatParameter:\n";
          std::map<std::string, std::string>::iterator it=_preset.begin();
          for(;it!=_preset.end();it++){
            result+="\t"+(*it).first+"="+(*it).second+"\n";
          }
        }
        if(_codecs.count("video")>0){
          result+="VideoCodec:\n";
          std::map<std::string, std::string>::iterator it=_codecs["video"].begin();
          for(;it!=_codecs["video"].end();it++){
            result+="\t"+(*it).first+"="+(*it).second+"\n";
          }
        }
        if(_codecs.count("audio")>0){
          result+="AudioCodec:\n";
          std::map<std::string, std::string>::iterator it=_codecs["audio"].begin();
          for(;it!=_codecs["audio"].end();it++){
            result+="\t"+(*it).first+"="+(*it).second+"\n";
          }
        }
        return result;
      }
    }
  }
}
