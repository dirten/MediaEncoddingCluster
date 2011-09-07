/* 
 * File:   PresetReaderJson.h
 * Author: HoelscJ
 *
 * Created on 23. Mai 2011, 16:24
 */

#ifndef PRESETREADERJSON_H
#define	PRESETREADERJSON_H

#include <string>
#include <map>
namespace org {
  namespace esb {
    namespace plugin {

      class PresetReaderJson {
      public:
        typedef std::map<std::string, std::multimap<std::string, std::string> > CodecList;
        typedef CodecList FilterList;
        typedef std::map<std::string, std::string> Preset;
        PresetReaderJson(std::string filename);
        virtual ~PresetReaderJson();
        Preset getPreset();
        CodecList getCodecList();
        FilterList getFilterList();
      private:
       CodecList _codecs;
       FilterList _filters;
       Preset _preset;

      };
    }
  }
}

#endif	/* PRESETREADERJSON_H */

