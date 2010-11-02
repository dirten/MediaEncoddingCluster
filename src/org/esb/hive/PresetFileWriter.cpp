/* 
 * File:   PresetFileWriter.cpp
 * Author: HoelscJ
 * 
 * Created on 2. November 2010, 16:15
 */

#include "PresetFileWriter.h"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include <iostream>
#include <ostream>

#include "org/esb/util/Log.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
namespace org {
  namespace esb {
    namespace hive {

      PresetFileWriter::PresetFileWriter(std::string filename):_filename(filename) {
      }

      PresetFileWriter::~PresetFileWriter() {
      }

      void PresetFileWriter::setPreset(Preset&preset) {
        _preset = preset;

      }

      void PresetFileWriter::setCodecList(CodecList&list) {
        _codecList = list;
      }

      void PresetFileWriter::setFilterList(FilterList&list) {
        _filterList = list;
      }

      void PresetFileWriter::write() {
        xml_node<> *presetnode = _doc.allocate_node(node_element, "preset");
        _doc.append_node(presetnode);

        xml_attribute<> *preset_name = _doc.allocate_attribute("name", _preset["name"].c_str());
        presetnode->append_attribute(preset_name);


        xml_node<> * format = _doc.allocate_node(node_element, "format");
        presetnode->append_node(format);
        xml_attribute<> *format_id = _doc.allocate_attribute("id", _preset["id"].c_str());
        format->append_attribute(format_id);
        Preset::iterator pit=_preset.begin();
        for(;pit!=_preset.end();pit++){
          if((*pit).first!="id"&&(*pit).first!="name"&&(*pit).first!="available_codecs"){
            buildParam(format,(*pit).first,(*pit).second);
          }
        }

        buildCodec(presetnode,"video");
        buildCodec(presetnode,"audio");

        xml_node<> * filters = _doc.allocate_node(node_element, "filters");
        presetnode->append_node(filters);

        FilterList::iterator fit=_filterList.begin();
        for(;fit!=_filterList.end();fit++){
          buildFilter(filters,(*fit).first);
        }
        
        std::stringstream t;
        t<<_doc;
        //std::cout << t.str();
        org::esb::io::File outfile(_filename);
        org::esb::io::FileOutputStream stream(&outfile);
        stream.write(t.str().insert(0,"<?xml version=\"1.0\"?>\n"));
        stream.close();
      }

      void PresetFileWriter::buildCodec(xml_node<>*parent,std::string type) {
        std::multimap<std::string, std::string> video=_codecList[type];
        xml_node<> * videoCodec = _doc.allocate_node(node_element, "codec");
        parent->append_node(videoCodec);
        xml_attribute<> *video_codec_type = _doc.allocate_attribute("type", _doc.allocate_string(type.c_str()));
        videoCodec->append_attribute(video_codec_type);
        if(video.count("codec_id")>0){
          xml_attribute<> *video_codec_id = _doc.allocate_attribute("id", (*video.find("codec_id")).second.c_str());
          videoCodec->append_attribute(video_codec_id);
        }else{
          LOGERROR("Could not find codec id for the "<<type<<" Codec");
        }
        std::multimap<std::string, std::string>::iterator vit=video.begin();
        for(;vit!=video.end();vit++){
          if((*vit).first!="codec_id"){
            buildParam(videoCodec,(*vit).first,(*vit).second);
          }
        }
      }
      void PresetFileWriter::buildFilter(xml_node<>*parent,std::string id) {
        std::multimap<std::string, std::string> filters=_filterList[id];
        xml_node<> * filter = _doc.allocate_node(node_element, "filter");
        parent->append_node(filter);
        
          xml_attribute<> *filter_id = _doc.allocate_attribute("id", id.c_str());
          filter->append_attribute(filter_id);
        std::multimap<std::string, std::string>::iterator vit=filters.begin();
        for(;vit!=filters.end();vit++){
          if((*vit).first!="id"){
            buildParam(filter,(*vit).first,(*vit).second);
          }
        }
      }

      void PresetFileWriter::buildParam(xml_node<>*parent, std::string name, std::string value) {
        
        xml_node<> * param = _doc.allocate_node(node_element, "param");
        parent->append_node(param);
        xml_attribute<> *namenode = _doc.allocate_attribute("name", _doc.allocate_string(name.c_str()));
        xml_attribute<> *valuenode = _doc.allocate_attribute("value", _doc.allocate_string(value.c_str()));

        param->append_attribute(namenode);
        param->append_attribute(valuenode);
        
      }

    }
  }
}
