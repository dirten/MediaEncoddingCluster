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
#include "exports.h"
namespace org {
  namespace esb {
    namespace hive {

      class HIVE_EXPORT PresetReaderJson {
      public:
        typedef std::map<std::string, std::map<std::string, std::string> > CodecList;
        typedef CodecList FilterList;
        typedef std::map<std::string, std::string> Preset;
        PresetReaderJson(std::string filename);
        ~PresetReaderJson();
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

