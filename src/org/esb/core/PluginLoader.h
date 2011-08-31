/* 
 * File:   PluginLoader.h
 * Author: HoelscJ
 *
 * Created on 29. August 2011, 15:48
 */

#ifndef PLUGINLOADER_H
#define	PLUGINLOADER_H
#include <string>
#include "exports.h"
namespace org {
  namespace esb {
    namespace core {

      class CORE_EXPORT PluginLoader {
      public:
        PluginLoader(std::string path);
        virtual ~PluginLoader();
      private:
        void load(std::string file);
      };
    }
  }
}

#endif	/* PLUGINLOADER_H */

