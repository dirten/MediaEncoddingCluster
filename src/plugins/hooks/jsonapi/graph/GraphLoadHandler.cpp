#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "../JSONResult.h"

#include "../exports.h"
#include "org/esb/util/Log.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"

class JSONAPI_EXPORT GraphLoadHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    LOGDEBUG("Loading graph");
    //res.setChunkedTransferEncoding(true);
    std::string user_path = req.get("hive.graph_path");

    JSONResult result(req);

    std::string uuid = req.get("uuid");
    org::esb::io::File f(user_path + "/" + uuid + ".graph");
    LOGDEBUG(f.getPath());
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
        result.setData(inode);
      }else{
        result.setStatus(res.HTTP_INTERNAL_SERVER_ERROR, "graph is invalid");
      }
    } else {
      result.setStatus(res.HTTP_NOT_FOUND, "flow not found");
    }
    std::ostream& ostr = res.send();
    res.setContentType("text/plain");
    ostr << result.write_formatted();
  }
};
REGISTER_WEB_HOOK("/api/v1/flow/{uuid}$", GET, GraphLoadHandler);
