//#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/config/config.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "../JSONResult.h"
#include "../exports.h"

class JSONAPI_EXPORT GraphListHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    res.setChunkedTransferEncoding(true);

    JSONResult result(req);
    std::string user_path = org::esb::config::Config::get("hive.graph_path");

    org::esb::io::File f(user_path + "/");
    org::esb::io::FileList files = f.listFiles();
    JSONNode data(JSON_ARRAY);

    foreach(Ptr<org::esb::io::File> file, files) {
      /**load graph file*/
      org::esb::io::FileInputStream fis(file.get());
      std::string ndata;
      fis.read(ndata);

      /**parsing json file*/
      if (libjson::is_valid(ndata)) {
        //LOGDEBUG("Data is valid");
        JSONNode inode = libjson::parse(ndata);
        std::string name;
        std::string uuid;
        if (inode.contains("name")) {
          name = inode["name"].as_string();
        }
        if (inode.contains("uuid")) {
          uuid = inode["uuid"].as_string();
        }
        JSONNode file_node(JSON_NODE);
        file_node.set_name("graph");
        file_node.push_back(JSONNode("uuid", uuid));
        file_node.push_back(JSONNode("name", name));
        data.push_back(file_node);
      }
    }
    result.setData(data);
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }
};
REGISTER_WEB_HOOK("/api/v1/flow$", GET, GraphListHandler)
