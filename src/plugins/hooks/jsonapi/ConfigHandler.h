/* 
 * File:   ConfigHandler.h
 * Author: HoelscJ
 *
 * Created on 17. Oktober 2011, 10:46
 */

#ifndef CONFIGHANDLER_H
#define	CONFIGHANDLER_H
#include "org/esb/core/HookPlugin.h"
#include "org/esb/libjson/JSONNode.h"
namespace api {

  class ConfigHandler: public org::esb::core::HookPlugin {
  public:
    ConfigHandler();
    virtual ~ConfigHandler();
    void handleRequest(org::esb::core::Request * req, org::esb::core::Response*res);
    org::esb::core::OptionsDescription getOptionsDescription();
    void init();
  private:
    std::string _base;
    int _uri_len;
    void handleListConfig(org::esb::core::Request * req, org::esb::core::Response*res);
    void handleSetConfig(org::esb::core::Request * req, org::esb::core::Response*res);
    void handleKey(std::string, JSONNode& );
    bool contains(JSONNode& node, std::string name);

  };
  REGISTER_HOOK("web.api.Service",ConfigHandler, ConfigHandler::handleRequest,1);
}

#endif	/* CONFIGHANDLER_H */

