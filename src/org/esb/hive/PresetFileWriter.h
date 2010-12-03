/* 
 * File:   PresetFileWriter.h
 * Author: HoelscJ
 *
 * Created on 2. November 2010, 16:15
 */

#ifndef PRESETFILEWRITER_H
#define	PRESETFILEWRITER_H
#include <map>
#include <string>
#include "rapidxml-1.13/rapidxml_print.hpp"

namespace org {
  namespace esb {
    namespace hive {
      using namespace rapidxml;
      class PresetFileWriter {
      public:
        typedef std::map<std::string, std::multimap<std::string, std::string> > CodecList;
        typedef CodecList FilterList;
        typedef std::map<std::string, std::string> Preset;

        PresetFileWriter(std::string filename);
        virtual ~PresetFileWriter();
        void setPreset(Preset&);
        void setCodecList(CodecList&);
        void setFilterList(FilterList&);
        void write();
        //std::string toString();
      private:
        std::string _filename;
        xml_document<> _doc;
        std::string _xml;
        CodecList _codecList;
        FilterList _filterList;
        Preset _preset;
        void buildParam(xml_node<>*parent, std::string name,std::string value);
        void buildCodec(xml_node<>*parent,std::string type);
        void buildFilter(xml_node<>*parent,std::string id);
      };
    }
  }
}

#endif	/* PRESETFILEWRITER_H */

