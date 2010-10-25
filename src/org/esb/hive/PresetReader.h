/* 
 * File:   PresetReader.h
 * Author: jholscher
 *
 * Created on 25. Oktober 2010, 23:14
 */
#include <string>
#include "boost/property_tree/detail/rapidxml.hpp"
#include <map>
#ifndef PRESETREADER_H
#define	PRESETREADER_H
namespace org {
  namespace esb {
    namespace hive {

      class PresetReader {
      public:
        PresetReader(std::string filename);
        virtual ~PresetReader();
        void loadIntoDB();
        std::string toString();
      private:
       void handleFormat(rapidxml::xml_node<> *);
       void handleCodec(rapidxml::xml_node<> *);
       void handleVideoCodec(rapidxml::xml_node<> *);
       void handleAudioCodec(rapidxml::xml_node<> *);
       void handleFilterList(rapidxml::xml_node<> *);
       void handleFilter(rapidxml::xml_node<> *);
       std::map<std::string, std::map<std::string, std::string> > _codecs;
       std::map<std::string, std::map<std::string, std::string> > _filters;
       std::map<std::string, std::string> _preset;
      };

    }
  }
}

#endif	/* PRESETREADER_H */

