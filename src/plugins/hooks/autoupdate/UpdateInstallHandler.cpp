#include "UpdateInstallHandler.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/libjson/JSONResult.h"
#include "org/esb/core/Version.h"

using org::esb::core::Version;

UpdateInstallHandler::UpdateInstallHandler()
{
}

void UpdateInstallHandler::handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res){
  JSONResult result(req);

  std::string version = req.get("version");
  Version v(version);
  LOGDEBUG("Installing version:"<<v)

  res.setContentType("text/plain");
  std::ostream& ostr = res.send();
  ostr << result.write_formatted();

}
