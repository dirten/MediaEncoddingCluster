#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/CountingStream.h"
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"

#include "../exports.h"

class GraphSubmitPartHandler : public Poco::Net::PartHandler {
public:

  void handlePart(const Poco::Net::MessageHeader&header, std::istream&stream) {
    LOGDEBUG("HandlePart");
    if(header.has("Content-Disposition")){
      LOGDEBUG("Content-Disposition");
      std::string disp;
      Poco::Net::NameValueCollection params;
      Poco::Net::MessageHeader::splitParameters(header["Content-Disposition"], disp,params);
      LOGDEBUG("Param-Name"<<params.get("name","(unnamed)"));
      LOGDEBUG("Param-FileName"<<params.get("filename","(unnamed)"));
    }
    Poco::CountingInputStream cistr(stream);
    Poco::NullOutputStream noss;
    Poco::StreamCopier::copyStream(cistr, noss);
  }

  std::string getData() {
    return _data;
  }
private:
  std::string _data;
};

class JSONAPI_EXPORT GraphSubmitHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    JSONNode result(JSON_NODE);
    result.push_back(JSONNode("requestUUID", req.get("requestUUID")));
    std::string uuid = req.get("uuid");
    LOGDEBUG("Submit Handler1212:" << uuid);
    //LOGDEBUG("Submit Handler1212:" <<req.get("file"));
    org::esb::io::File f(req.get("user_path") + "/" + uuid + ".graph");
    if (f.exists()) {
      LOGDEBUG("ContentLength"<<req.getContentLength());
      /*when  POST Data arrived*/
      if(req.getContentLength()>0){
        GraphSubmitPartHandler partHandler;
        Poco::Net::HTMLForm form(req, req.stream(), partHandler);
      }
      result.push_back(JSONNode("status", "ok"));
    } else {
      result.push_back(JSONNode("status", "error"));
      result.push_back(JSONNode("message", "graph for submission not found!"));
      res.setStatusAndReason(res.HTTP_NOT_FOUND, "graph for submission not found!");
    }
    res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }
};
REGISTER_WEB_HOOK("/api/v1/graph/{uuid}/submit", PUT, GraphSubmitHandler);



