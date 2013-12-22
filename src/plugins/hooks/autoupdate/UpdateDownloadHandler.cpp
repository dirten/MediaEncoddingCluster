#include "UpdateDownloadHandler.h"
#include "org/esb/io/File.h"
#include "org/esb/hive/Environment.h"
#include "org/esb/util/Log.h"

#include "Poco/StreamCopier.h"
#include "Poco/Net/HTTPResponse.h"
using org::esb::io::File;
using org::esb::hive::Environment;
using Poco::StreamCopier;
//using Poco::Net::HTTPResponse::HTTPStatus;
UpdateDownloadHandler::UpdateDownloadHandler()
{
}

void UpdateDownloadHandler::handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res){

  std::string filename = req.get("1");

  std::string update_path_str=Environment::get("hive.update_path");
  update_path_str+="/"+filename;
  LOGDEBUG("looking for update file : "<<update_path_str)

  File update_file(update_path_str);
  if(update_file.exists()){
    LOGDEBUG("file exist");
    res.setStatus(res.HTTP_OK);
    res.setChunkedTransferEncoding(true);
    res.set("Content-Disposition","update");
    std::ifstream ifs (update_path_str.c_str(), std::ifstream::in);
    std::ostream& ostr = res.send();
    StreamCopier::copyStream64(ifs, ostr);
  }else{
    res.setStatusAndReason(res.HTTP_NOT_FOUND,"file not found");
    std::ostream& ostr = res.send();
    ostr.flush();
  }
}
