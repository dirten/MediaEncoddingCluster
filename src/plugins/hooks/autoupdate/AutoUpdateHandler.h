#ifndef AUTOUPDATEHANDLER_H
#define AUTOUPDATEHANDLER_H

#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/io/File.h"

using org::esb::io::File;
class AutoUpdateHandler : public org::esb::core::WebHookPlugin
{
  public:
    AutoUpdateHandler();
    void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res);
  private:
    static bool sortVersion( Ptr<File> left,  Ptr<File> right);
};
REGISTER_WEB_HOOK("/api/v1/update$", GET, AutoUpdateHandler)

#endif // AUTOUPDATEHANDLER_H
