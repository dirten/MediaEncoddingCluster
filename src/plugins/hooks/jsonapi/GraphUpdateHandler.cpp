#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/CountingStream.h"
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"

#include "exports.h"

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
    JSONNode result(JSON_NODE);
    result.push_back(JSONNode("requestUUID", req.get("requestUUID")));
    if (req.getContentLength() < 1024 * 1024) {
      std::string uuid = req.get("uuid");
      org::esb::io::File f(req.get("user_path") + "/" + uuid + ".graph");
      if (f.exists()) {
        GraphPartHandler partHandler;
        Poco::Net::HTMLForm form(req, req.stream(), partHandler);
        if (libjson::is_valid(partHandler.getData())) {
          JSONNode inode = libjson::parse(partHandler.getData());
          if (inode.contains("uuid")) {
            uuid = inode["uuid"].as_string();
          } else {
            inode.push_back(JSONNode("uuid", uuid));
          }
          /*save method should here*/
          save(inode, uuid, req.get("user_path"));
          result.push_back(JSONNode("status","ok"));
          result.push_back(inode);
          //result = inode;
        } else {
          JSONNode error(JSON_NODE);
          error.set_name("error");
          error.push_back(JSONNode("code", "parse_error"));
          error.push_back(JSONNode("description", "no valid json format given"));
          result.push_back(error);
        }
      }else{
        result.push_back(JSONNode("status", "error"));
        result.push_back(JSONNode("message", "graph for update not found!"));
        res.setStatusAndReason(res.HTTP_NOT_FOUND, "graph for update not found!");
      }
    } else {
      res.setChunkedTransferEncoding(false);
      result.push_back(JSONNode("status", "error"));
      result.push_back(JSONNode("message", "Request size to big!"));
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
REGISTER_WEB_HOOK("/api/v1/graph/{uuid}", PUT, GraphUpdateHandler);

