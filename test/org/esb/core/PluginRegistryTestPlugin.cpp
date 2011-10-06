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


PluginRegistryTestPlugin2::~PluginRegistryTestPlugin2() {
  std::cout << "destructor ~PluginRegistryTestPlugin2()" << std::endl;

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



PluginRegistryTestPlugin4::PluginRegistryTestPlugin4() {
  std::cout << "hier ist das plugin 4" << std::endl;
}

PluginRegistryTestPlugin4::~PluginRegistryTestPlugin4() {
  std::cout << "destructor ~PluginRegistryTestPlugin4()" << std::endl;
}
