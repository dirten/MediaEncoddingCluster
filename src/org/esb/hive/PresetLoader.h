/* 
 * File:   PresetToDB.h
 * Author: HoelscJ
 *
 * Created on 26. Oktober 2010, 11:08
 */

#ifndef PRESETLOADER_H
#define	PRESETLOADER_H
#include "org/esb/db/hivedb.hpp"
#include "PresetReader.h"
namespace org {
  namespace esb {
    namespace hive {

      class PresetLoader {
      public:
        PresetLoader(PresetReader & preset);
        virtual ~PresetLoader();
        void load();
      private:
        PresetReader & _preset;
        void setParameter(db::Profile p,std::string key, std::string val);
      };
    }
  }
}

#endif	/* PRESETTODB_H */

