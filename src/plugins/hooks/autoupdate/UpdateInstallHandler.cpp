#include "UpdateInstallHandler.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/libjson/JSONResult.h"
#include "org/esb/core/Version.h"
#include "org/esb/hive/Environment.h"
#include "org/esb/io/File.h"

#include "Poco/Net/HTTPResponse.h"

using org::esb::core::Version;
using org::esb::hive::Environment;
using org::esb::io::File;
UpdateInstallHandler::UpdateInstallHandler()
{
}

void UpdateInstallHandler::handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res){
  JSONResult result(req);

  std::string version = req.get("version");
  Version v(version);
  Version current_version;

  if(!v.equals(current_version)){
    std::string filename=std::string("MediaEncodingCluster-").
    append(v.toString()).append("-").
    append(Environment::get(Environment::SYSTEM)).
    append(".tar.gz");

    std::string update_path_str=Environment::get("hive.update_path");
    update_path_str.append("/").append(filename);
    LOGDEBUG("try installing update :"<<update_path_str)

    File updateFile(update_path_str);
    if(updateFile.exists()&&updateFile.isFile()){
      LOGDEBUG("unpacking file :"<<update_path_str)
    }else{
      LOGWARN("update file not found :"<<update_path_str)
      result.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND,"Update File not found","");
    }
  }else{
    LOGWARN("Update is the same Version as Current:"<<v.toString())
    result.setStatus(Poco::Net::HTTPResponse::HTTP_CONFLICT,"Update is the same Version as Current","");
  }
  res.setContentType("text/plain");
  std::ostream& ostr = res.send();
  ostr << result.write_formatted();

}
