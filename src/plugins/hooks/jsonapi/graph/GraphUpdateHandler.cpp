#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/CountingStream.h"
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/NetException.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "../JSONResult.h"

#include "../exports.h"

class GraphPartHandler : public Poco::Net::PartHandler {
public:

  void handlePart(const Poco::Net::MessageHeader&header, std::istream&stream) {
    Poco::CountingInputStream cistr(stream);
    Poco::StreamCopier::copyToString(cistr, _data);
  }

  std::string getData() {
    return _data;
  }
private:
  std::string _data;
};

class JSONAPI_EXPORT GraphUpdateHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    JSONResult result(req);
    if (req.getContentLength() == 0) {
      res.setChunkedTransferEncoding(false);
      result.setStatus("error", "Request could not be empty!");
      res.setStatusAndReason(res.HTTP_BAD_REQUEST, "Request could not be empty!");
    } else
      if (req.getContentLength() < 1024 * 1024) {
      std::string uuid = req.get("uuid");
      org::esb::io::File f(req.get("hive.graph_path") + "/" + uuid + ".graph");
      if (f.exists()) {
        std::string data;
        Poco::StreamCopier::copyToString(req.stream(), data);
        if (libjson::is_valid(data)) {
          JSONNode inode = libjson::parse(data);
          if (inode.contains("uuid")) {
            uuid = inode["uuid"].as_string();
          } else {
            inode.push_back(JSONNode("uuid", uuid));
          }
          /*save method should here*/
          save(inode, uuid, req.get("hive.graph_path"));
        } else {
          result.setStatus("error", "no valid json format given!");
          res.setStatusAndReason(res.HTTP_BAD_REQUEST, "no valid json format given!");
        }
      } else {
        result.setStatus("error", "graph for update not found!");
        res.setStatusAndReason(res.HTTP_NOT_FOUND, "graph for update not found!");
      }
    } else {
      res.setChunkedTransferEncoding(false);
      result.setStatus("error", "Request size to big!");
      res.setStatusAndReason(res.HTTP_BAD_REQUEST, "Request size to big!");
    }
    //throw Poco::Net::HTTPException("Tets Exception",res.HTTP_BAD_REQUEST);
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
REGISTER_WEB_HOOK("/api/v1/flow/{uuid}", POST, GraphUpdateHandler);

