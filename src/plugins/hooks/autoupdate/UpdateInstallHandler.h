#ifndef UPDATEINSTALLHANDLER_H
#define UPDATEINSTALLHANDLER_H
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/util/Log.h"

class UpdateInstallHandler: public org::esb::core::WebHookPlugin
{
classlogger("UpdateInstallHandler")
public:
  UpdateInstallHandler();
  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res);
};

REGISTER_WEB_HOOK("/api/v1/update/{version}$", POST, UpdateInstallHandler)

#endif // UPDATEINSTALLHANDLER_H
