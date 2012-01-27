#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"

#include "exports.h"
#include "org/esb/util/Log.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"

class JSONAPI_EXPORT GraphLoadHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    LOGDEBUG("Loading graph");
    res.setChunkedTransferEncoding(true);
    res.setContentType("text/plain");
    std::string user_path = req.get("user_path");

    std::ostream& ostr = res.send();
    JSONNode result(JSON_NODE);
    result.push_back(JSONNode("requestUUID", req.get("requestUUID")));

    std::string uuid = req.get("uuid");
    LOGDEBUG("uuid="<<uuid);
    org::esb::io::File f(user_path + "/" + uuid + ".graph");
    if (f.exists()) {
      org::esb::io::FileInputStream fis(&f);
      std::string ndata;
      fis.read(ndata);

      /**parsing json file*/
      if (libjson::is_valid(ndata)) {
        LOGDEBUG("Data is valid");
        JSONNode inode = libjson::parse(ndata);
        if (inode.contains("uuid")) {
          uuid = inode["uuid"].as_string();
        } 
        result.push_back(JSONNode("status", "ok"));
        inode.set_name("data");
        result.push_back(inode);
      }else{
        result.push_back(JSONNode("status", "error"));
        result.push_back(JSONNode("message", "graph is invalid"));
        
      }
    } else {
      result.push_back(JSONNode("status", "error"));
      result.push_back(JSONNode("message", "graph not found"));
    }
    ostr << result.write_formatted();
  }
};
REGISTER_WEB_HOOK("/api/v1/graph/{uuid}[^/]", GET, GraphLoadHandler);
