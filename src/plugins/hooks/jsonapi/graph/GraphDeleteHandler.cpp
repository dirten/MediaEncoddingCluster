#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/CountingStream.h"
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "../JSONResult.h"

#include "../exports.h"

class JSONAPI_EXPORT GraphDeleteHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    JSONResult result(req);
    std::string uuid = req.get("uuid");
    org::esb::io::File f(req.get("user_path") + "/" + uuid + ".graph");
    if (f.exists()) {
      f.deleteFile();
      result.push_back(JSONNode("uuid", uuid));
    } else {
      result.setStatus("message", "graph for delete not found!");
      res.setStatusAndReason(res.HTTP_NOT_FOUND, "graph for delete not found!");
    }
    res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }
};
REGISTER_WEB_HOOK("/api/v1/flow/{uuid}", DELETE, GraphDeleteHandler);


