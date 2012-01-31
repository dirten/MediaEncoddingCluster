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
#include "org/esb/util/Foreach.h"
#include "../JSONResult.h"

#include "../exports.h"

class JSONAPI_EXPORT FileHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    JSONResult result(req);
    if (req.has("path")) {
      std::string path = req.get("path");
      result.push_back(JSONNode("path", req.get("path")));
      org::esb::io::File f(path);
      if (f.isDirectory()) {
        org::esb::io::FileList flist = f.listFiles();
        JSONNode array(JSON_ARRAY);
        array.set_name("data");
        foreach(org::esb::io::FileList::value_type &file, flist) {
          buildFile(*file.get(), array);
        }
        result.push_back(array);
      } else
        if (f.isFile()) {
        JSONNode array(JSON_ARRAY);
        array.set_name("data");
        buildFile(f, array);
        result.push_back(array);
      } else {
        result.push_back(JSONNode("error", "file not found"));
      }
    }
    res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }

  void buildFile(org::esb::io::File & file, JSONNode & node) {
    JSONNode data(JSON_NODE);
    data.push_back(JSONNode("path", file.getFilePath()));
    data.push_back(JSONNode("name", file.getFileName()));
    data.push_back(JSONNode("directory", file.isDirectory()));
    node.push_back(data);
  }

};
REGISTER_WEB_HOOK("/api/v1/file/?{path}?", GET, FileHandler);



