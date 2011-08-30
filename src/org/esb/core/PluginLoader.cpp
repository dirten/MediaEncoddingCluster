/* 
 * File:   PluginLoader.cpp
 * Author: HoelscJ
 * 
 * Created on 29. August 2011, 15:48
 */

#include "PluginLoader.h"
#include "org/esb/io/File.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/lang/SharedObjectLoader.h"
namespace org {
  namespace esb {
    namespace core {

      PluginLoader::PluginLoader(std::string path) {
        org::esb::io::File plugin_dir(path);
        if (plugin_dir.isDirectory()) {
          org::esb::io::FileList plugin_list = plugin_dir.listFiles();

          foreach(Ptr<org::esb::io::File> f, plugin_list) {
            if (f->isFile())
              load(f->getPath());
          }
        } else if (plugin_dir.isFile()) {
          load(plugin_dir.getPath());
        }
      }

      void PluginLoader::load(std::string file) {
        org::esb::lang::SharedObjectLoader * loader = new org::esb::lang::SharedObjectLoader(file);
      }

      PluginLoader::~PluginLoader() {
      }
    }
  }
}