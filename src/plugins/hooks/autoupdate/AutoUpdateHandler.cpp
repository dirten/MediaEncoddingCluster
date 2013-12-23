#include "AutoUpdateHandler.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/libjson/JSONResult.h"

#include "org/esb/hive/Environment.h"
#include "org/esb/util/Foreach.h"

using org::esb::io::File;
using org::esb::io::FileList;
using org::esb::hive::Environment;

AutoUpdateHandler::AutoUpdateHandler()
{
}

bool AutoUpdateHandler::sortVersion(Ptr<File> left, Ptr<File> right){
  return left->getFileName().compare(right->getFileName())>0;
}

void AutoUpdateHandler::handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res){

  std::string update_path_str=Environment::get("hive.update_path");
  File update_path(update_path_str);

  FileList updates=update_path.listFiles();
  updates.sort(sortVersion);
  std::string version;

  if(updates.size()>0){
    version=updates.front()->getFileName().substr(21,7);
  }

  res.setContentType("text/plain");
  std::ostream& ostr = res.send();
  ostr << version;//result.write_formatted();

}
