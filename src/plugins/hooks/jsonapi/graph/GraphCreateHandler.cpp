#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/CountingStream.h"
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/util/UUID.h"
#include "../JSONResult.h"

#include "../exports.h"

class JSONAPI_EXPORT GraphCreateHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    JSONResult result(req);
    if (req.getContentLength() < 1024 * 1024) {
      std::string uuid = org::esb::util::PUUID();

      std::string data;
      Poco::StreamCopier::copyToString(req.stream(), data);
      if (libjson::is_valid(data)) {
        JSONNode inode = libjson::parse(data);
        if (inode.contains("uuid")) {
          JSONNode tmp=JSONNode("uuid", uuid);
          inode["uuid"].swap(tmp);
        } else {
          inode.push_back(JSONNode("uuid", uuid));
        }
        /*save method should here*/
        save(inode, uuid, req.get("hive.graph_path"));
        result.setStatus(res.HTTP_OK,"Flow Successful created.");
        result.push_back(JSONNode("uuid", uuid));
        //result = inode;
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

  void save(JSONNode& node, std::string & uuid, std::string path) {
    org::esb::io::File f(path + "/" + uuid + ".graph");
    org::esb::io::FileOutputStream fos(&f);
    fos.write(node.write_formatted());
  }
};
REGISTER_WEB_HOOK("/api/v1/flow", POST, GraphCreateHandler);


