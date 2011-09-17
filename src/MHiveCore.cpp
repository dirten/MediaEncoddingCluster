
#include "config.h"
#include "org/esb/config/config.h"
#include "org/esb/io/File.h"

#include "org/esb/lang/CtrlCHitWaiter.h"
#include "org/esb/core/PluginRegistry.h"

int main(int argc, char** argv) {
    /*initialise the config class*/
  org::esb::config::Config::init();
  org::esb::io::File f(argv[0]);
  std::string base_path = org::esb::io::File(f.getParent()).getParent();
  //Log::open(base_path+"/res");

  org::esb::core::PluginRegistry::getInstance()->load(base_path+"/plugins");
  org::esb::core::PluginRegistry::getInstance()->startServices();
  org::esb::lang::CtrlCHitWaiter::wait();
  org::esb::core::PluginRegistry::getInstance()->stopServices();
  org::esb::core::PluginRegistry::close();
}