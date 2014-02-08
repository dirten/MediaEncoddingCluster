#include "UpdateInstallHandler.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/libjson/JSONResult.h"

UpdateInstallHandler::UpdateInstallHandler()
{
}
void UpdateInstallHandler::handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res){
  JSONResult result(req);

  std::string version = req.get("version");


  res.setContentType("text/plain");
  std::ostream& ostr = res.send();
  ostr << result.write_formatted();

}
