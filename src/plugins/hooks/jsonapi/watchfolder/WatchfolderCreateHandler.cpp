#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/libjson/JSONResult.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/CountingStream.h"
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/util/UUID.h"

#include "../exports.h"

class JSONAPI_EXPORT WatchfolderCreateHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    JSONResult result(req);
    if (req.getContentLength() < 1024 * 1024) {
      std::string uuid = org::esb::util::PUUID();

      std::string data;
      Poco::StreamCopier::copyToString(req.stream(), data);
      if (libjson::is_valid(data)) {
        JSONNode inode = libjson::parse(data);
        if(!inode.contains("folder")){
          
          result.setStatus("error", "no folder given, folder element could not be empty");
          res.setStatusAndReason(res.HTTP_BAD_REQUEST, "no valid json format given");
          
        }
        result.push_back(JSONNode("uuid", uuid));
      } else {
        res.setChunkedTransferEncoding(false);
        result.setStatus("error", "no valid json format given");
        res.setStatusAndReason(res.HTTP_BAD_REQUEST, "no valid json format given");
      }
    } else {
      res.setChunkedTransferEncoding(false);
      result.setStatus("error", "Request size to big!");
      res.setStatusAndReason(res.HTTP_BAD_REQUEST, "Request size to big!");
    }
    res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }
};
REGISTER_WEB_HOOK("/api/v1/watchfolder/?$", POST, WatchfolderCreateHandler);
