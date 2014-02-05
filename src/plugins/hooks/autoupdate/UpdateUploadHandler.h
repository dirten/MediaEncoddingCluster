#ifndef UPDATEUPLOADHANDLER_H
#define UPDATEUPLOADHANDLER_H

#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/io/File.h"

using org::esb::io::File;
class UpdateUploadHandler : public org::esb::core::WebHookPlugin
{
  public:
    UpdateUploadHandler();
    void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res);
};
REGISTER_WEB_HOOK("/api/v1/update$", POST, UpdateUploadHandler)

#endif // UPDATEUPLOADHANDLER_H
