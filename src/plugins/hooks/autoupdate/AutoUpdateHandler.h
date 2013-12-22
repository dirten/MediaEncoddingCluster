#ifndef AUTOUPDATEHANDLER_H
#define AUTOUPDATEHANDLER_H

#include "org/esb/core/WebHookPlugin.h"

class AutoUpdateHandler : public org::esb::core::WebHookPlugin
{
public:
  AutoUpdateHandler();
  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res);
};
REGISTER_WEB_HOOK("/api/v1/update$", GET, AutoUpdateHandler)

#endif // AUTOUPDATEHANDLER_H
