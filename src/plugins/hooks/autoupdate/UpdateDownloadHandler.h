#ifndef UPDATEDOWNLOADHANDLER_H
#define UPDATEDOWNLOADHANDLER_H
#include "org/esb/core/WebHookPlugin.h"

class UpdateDownloadHandler: public org::esb::core::WebHookPlugin
{
public:
  UpdateDownloadHandler();
  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res);

};
REGISTER_WEB_HOOK("/api/v1/update/?([\\w-/:\\.]+)$", GET, UpdateDownloadHandler)

#endif // UPDATEDOWNLOADHANDLER_H
