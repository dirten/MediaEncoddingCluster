/* 
 * File:   PluginRegistryTestPlugin.h
 * Author: HoelscJ
 *
 * Created on 29. August 2011, 14:55
 */

#ifndef PLUGINREGISTRYTESTPLUGIN_H
#define	PLUGINREGISTRYTESTPLUGIN_H

#include "org/esb/core/Plugin.h"
#include "org/esb/core/ServicePlugin.h"
#include "org/esb/core/WebservicePlugin.h"
#include "org/esb/core/HookPlugin.h"
#include "org/esb/core/HookProvider.h"

class PluginRegistryTestPlugin : public org::esb::core::Plugin{
public:
  PluginRegistryTestPlugin();
  virtual ~PluginRegistryTestPlugin();
private:

};

class PluginRegistryTestPlugin2 : public org::esb::core::ServicePlugin{
public:
  PluginRegistryTestPlugin2();
  virtual ~PluginRegistryTestPlugin2();
  void startService();
  void stopService();
private:

};

class PluginRegistryTestPlugin3 : public org::esb::core::WebservicePlugin{
public:
  PluginRegistryTestPlugin3();
  virtual ~PluginRegistryTestPlugin3();
  void handle(org::esb::api::ServiceRequest *, org::esb::api::ServiceResponse *);
          void hook(){};

private:

};

class PluginRegistryTestPlugin4 : public org::esb::core::HookPlugin{
public:
  PluginRegistryTestPlugin4();
  virtual ~PluginRegistryTestPlugin4();
  void hook(void*,void*){}
};


REGISTER_SERVICE("test_plugin2", PluginRegistryTestPlugin2)
//REGISTER_HOOK("webserver.url.hook","/api/v1",PluginRegistryTestPlugin3)
REGISTER_HOOK("webserver.url.hook",PluginRegistryTestPlugin4,PluginRegistryTestPlugin4::hook,1)

        
#endif	/* PLUGINREGISTRYTESTPLUGIN_H */
