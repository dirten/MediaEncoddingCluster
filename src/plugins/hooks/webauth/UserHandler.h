/* 
 * File:   UserHandler.h
 * Author: HoelscJ
 *
 * Created on 11. Oktober 2011, 11:02
 */

#ifndef USERHANDLER_H
#define	USERHANDLER_H
#include "org/esb/core/HookPlugin.h"
namespace webauth {
  using namespace org::esb::core;
  class UserHandler:public HookPlugin {
  public:
    UserHandler();
    virtual ~UserHandler();
    void handleRequest(Request * req, Response*res);
    OptionsDescription getOptionsDescription();
    void init();

  private:

  };
  REGISTER_HOOK("web.api.Auth",UserHandler, UserHandler::handleRequest, 1);
}

#endif	/* USERHANDLER_H */

