/* 
 * File:   PluginRegistryTestPlugin.cpp
 * Author: HoelscJ
 * 
 * Created on 29. August 2011, 14:55
 */

#include "PluginRegistryTestPlugin.h"
#include <iostream>

PluginRegistryTestPlugin::PluginRegistryTestPlugin() {
  std::cout << "hier ist das plugin" << std::endl;
}

PluginRegistryTestPlugin::~PluginRegistryTestPlugin() {
  std::cout << "destructor ~PluginRegistryTestPlugin()" << std::endl;
}

PluginRegistryTestPlugin2::PluginRegistryTestPlugin2() {
  std::cout << "hier ist das plugin 2" << std::endl;
}

void PluginRegistryTestPlugin::setContext(org::esb::core::AppContext*) {

}

PluginRegistryTestPlugin2::~PluginRegistryTestPlugin2() {
  std::cout << "destructor ~PluginRegistryTestPlugin2()" << std::endl;

}

void PluginRegistryTestPlugin2::setContext(org::esb::core::AppContext*) {

}

void PluginRegistryTestPlugin2::startService() {
  std::cout << "hier ist das plugin 2 start signal" << std::endl;
}

void PluginRegistryTestPlugin2::stopService() {
  std::cout << "hier ist das plugin 2 stop signal" << std::endl;
}

PluginRegistryTestPlugin3::PluginRegistryTestPlugin3() {
  std::cout << "hier ist das plugin 3" << std::endl;
}

PluginRegistryTestPlugin3::~PluginRegistryTestPlugin3() {
  std::cout << "destructor ~PluginRegistryTestPlugin3()" << std::endl;
}

void PluginRegistryTestPlugin3::handle(org::esb::api::ServiceRequest *, org::esb::api::ServiceResponse *) {

}

void PluginRegistryTestPlugin3::setContext(org::esb::core::AppContext*) {

}
