#include "AutoUpdateHandler.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/libjson/JSONResult.h"

#include "org/esb/io/File.h"
#include "org/esb/hive/Environment.h"
#include "org/esb/util/Foreach.h"

using org::esb::io::File;
using org::esb::io::FileList;
using org::esb::hive::Environment;

AutoUpdateHandler::AutoUpdateHandler()
{
}

void AutoUpdateHandler::handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res){
  JSONResult result(req);
  JSONNode update_node(JSON_ARRAY);
  update_node.set_name("updates");

  std::string update_path_str=Environment::get("hive.update_path");
  File update_path(update_path_str);

  FileList updates=update_path.listFiles();
  foreach (Ptr<File> update, updates) {
    update_node.push_back(JSONNode("file",update->getFileName()));
  }

  result.push_back(update_node);
  res.setContentType("text/plain");
  std::ostream& ostr = res.send();
  ostr << result.write_formatted();

}
