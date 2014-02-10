#include "UpdateInstallHandler.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/libjson/JSONResult.h"
#include "org/esb/core/Version.h"
#include "org/esb/hive/Environment.h"
#include "org/esb/io/File.h"

#include "Poco/Net/HTTPResponse.h"
#include "Poco/Zip/Decompress.h"
#include "Poco/Delegate.h"
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
    append(version).append("-").
    append(Environment::get(Environment::SYSTEM)).
    append(".tar.gz");

    std::string update_path_str=Environment::get("hive.update_path");
    update_path_str.append("/").append(filename);
    LOGDEBUG("try installing update :"<<update_path_str)

    File updateFile(update_path_str);
    if(updateFile.exists()&&updateFile.isFile()){
      LOGDEBUG("unpacking file :"<<update_path_str<<" to "<<Poco::Path().toString())

      std::ifstream inp(update_path_str.c_str(), std::ios::binary);
      // decompress to current working dir
      Poco::Zip::Decompress dec(inp, Poco::Path());
      // if an error happens invoke the ZipTest::onDecompressError method
      dec.EError += Poco::Delegate<UpdateInstallHandler, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &UpdateInstallHandler::onDecompressError);
      try{
      dec.decompressAllFiles();
      }catch(std::exception & ex){
        LOGERROR("error decompression:"<<ex.what())
      }

      dec.EError -= Poco::Delegate<UpdateInstallHandler, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &UpdateInstallHandler::onDecompressError);

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
void UpdateInstallHandler::onDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info)
{
  LOGERROR("error extracting:"<<info.second)
}
