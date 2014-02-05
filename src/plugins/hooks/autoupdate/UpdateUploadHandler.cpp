#include "UpdateUploadHandler.h"
#include "org/esb/hive/Environment.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/StreamCopier.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/libjson/JSONResult.h"


using Poco::StreamCopier;
using org::esb::hive::Environment;
class UpdateUploadPartHandler : public Poco::Net::PartHandler {

public:
    UpdateUploadPartHandler(){
  }
  void handlePart(const Poco::Net::MessageHeader&header, std::istream&stream) {
    LOGDEBUG("HandlePart");
    Poco::Net::NameValueCollection::ConstIterator it=header.begin();
    for(;it!=header.end();it++){
      LOGDEBUG("Header key:"<<it->first)
    }

    std::string filename;
    if (header.has("Content-Disposition")) {
      LOGDEBUG("Content-Disposition");
      std::string disp;
      Poco::Net::NameValueCollection params;
      Poco::Net::MessageHeader::splitParameters(header["Content-Disposition"], disp, params);
      LOGDEBUG("Param-Name" << params.get("name", "(unnamed)"));
      LOGDEBUG("Param-FileName" << params.get("filename", "(unnamed)"));
      filename=params.get("filename", "(unnamed)");
    }
    if (header.has("Content-Length")) {
      LOGDEBUG("Content-Length"<<header.get("Content-Length"));
    }
    if(filename.length()){
      std::string update_path_str=Environment::get("hive.update_path");
      std::ofstream ostream(update_path_str.append("/").append(filename).c_str());
      StreamCopier::copyStream64(stream, ostream);
    }
  }

  ~UpdateUploadPartHandler(){
    LOGDEBUG("~UpdateUploadPartHandler()")
  }

  std::string getData() {
    return _data;
  }
  uint64_t getLength(){
    return _length;
  }

private:
  std::string _data;
  uint64_t _length;
};


UpdateUploadHandler::UpdateUploadHandler()
{
}

void UpdateUploadHandler::handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res){
  JSONResult result(req);

  std::string update_path_str=Environment::get("hive.update_path");
  //File update_path(update_path_str);
  try{
    //std::ofstream ostream(update_path_str.append("/test.upload").c_str());
   //release this scope before the graph will be freed
    UpdateUploadPartHandler partHandler;
    Poco::Net::HTMLForm form(req, req.stream(), partHandler);
  }catch(...){
    LOGDEBUG("upload canceled");
  }

  res.setContentType("text/plain");
  std::ostream& ostr = res.send();
  ostr << result.write_formatted();

}
