/* 
 * File:   Application.cpp
 * Author: HoelscJ
 * 
 * Created on 2. September 2011, 11:13
 */

#include "Application.h"
#include "org/esb/io/File.h"
#include <iostream>
#include <memory.h>
namespace org {
  namespace esb {
    namespace core {

      Application::Application(int argc, char * argv[]) {
        org::esb::io::File f(argv[0]);
        std::string base_path = org::esb::io::File(f.getParent()).getParent();
        org::esb::io::File conf_file(base_path + "/mhive.conf");
        std::cout << "loading config file" << conf_file.getPath() << std::endl;
        if (conf_file.exists()) {

        }
      }

      Application::~Application() {
      }
    }
  }
}

std::string getConfig(std::string key) {
  return getenv(key.c_str()) != NULL ? getenv(key.c_str()) : "";
}

void putConfig(std::string key, std::string val) {
  std::string env=std::string(key).append("=").append(val);
  char * pa=new char[env.length()+1];
  memset(pa,0,env.length()+1);
  memcpy(pa,env.c_str(),env.length());
  putenv(pa);
}
