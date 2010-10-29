/* 
 * File:   PresetReader.h
 * Author: jholscher
 *
 * Created on 25. Oktober 2010, 23:14
 */
#include <string>
#include "boost/property_tree/detail/rapidxml.hpp"
#include <map>
#include "org/esb/util/Log.h"
#ifndef PRESETREADER_H
#define	PRESETREADER_H
namespace org {
  namespace esb {
    namespace hive {

      class PresetReader {
        classlogger("org.esb.hive.PresetReader")
      public:
        typedef std::map<std::string, std::multimap<std::string, std::string> > CodecList;
        typedef CodecList FilterList;
        typedef std::map<std::string, std::string> Preset;
        PresetReader(std::string filename);
        Preset getPreset();
        CodecList getCodecList();
        FilterList getFilterList();
        virtual ~PresetReader();
        std::string toString();
      private:
       void handleFormat(rapidxml::xml_node<> *);
       void handleCodec(rapidxml::xml_node<> *);
       void handleFilter(rapidxml::xml_node<> *);
       CodecList _codecs;
       FilterList _filters;
       Preset _preset;
      };
    }
  }
}

#endif	/* PRESETREADER_H */

