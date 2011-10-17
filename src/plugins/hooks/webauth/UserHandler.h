/* 
 * File:   UserHandler.h
 * Author: HoelscJ
 *
 * Created on 11. Oktober 2011, 11:02
 */

#ifndef USERHANDLER_H
#define	USERHANDLER_H
#include "org/esb/core/HookPlugin.h"
#include <map>
#include <list>
namespace webauth {
  using namespace org::esb::core;
  class UserHandler:public HookPlugin {
  public:
    UserHandler();
    virtual ~UserHandler();
    void handleAuthorization(Request * req, Response*res);
    void handleUpdate(Request * req, Response*res);
    void handleRegistration(Request * req, Response*res);
    OptionsDescription getOptionsDescription();
    void init();

  private:
    std::map<std::string, int> _user_map;
    std::list<std::string> _protected_uris;
  };
  REGISTER_HOOK("web.api.Auth",UserHandler, UserHandler::handleRegistration, 1);
  REGISTER_HOOK("web.api.Auth",UserHandler, UserHandler::handleAuthorization, 2);
  REGISTER_HOOK("web.api.Auth",UserHandler, UserHandler::handleUpdate, 3);
}

#endif	/* USERHANDLER_H */

