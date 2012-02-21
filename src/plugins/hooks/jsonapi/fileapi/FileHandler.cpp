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
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>


class JSONAPI_EXPORT FileHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    JSONResult result(req);
    if (req.has("1")) {
      std::string path = req.get("1");
      LOGDEBUG("Path find /:" << path.find("/"));
#ifndef __WIN32__
      if (path.find("/") != 0) {
        path = "/" + path;
      }
#endif

      result.push_back(JSONNode("path", path));
      org::esb::io::File f(path);




#ifdef WIN32
      LOGDEBUG("Windows Path:" << path);
      if (path == "/") {
        LOGDEBUG("building drives for windows");
        std::set<std::string> drives;
        drives.insert("A:\\");
        drives.insert("B:\\");
        drives.insert("C:\\");
        drives.insert("D:\\");
        drives.insert("E:\\");
        drives.insert("F:\\");
        drives.insert("G:\\");
        drives.insert("H:\\");
        drives.insert("I:\\");
        drives.insert("J:\\");
        drives.insert("K:\\");
        drives.insert("L:\\");
        drives.insert("M:\\");
        drives.insert("N:\\");
        drives.insert("O:\\");
        drives.insert("P:\\");
        drives.insert("Q:\\");
        drives.insert("R:\\");
        drives.insert("S:\\");
        drives.insert("T:\\");
        drives.insert("U:\\");
        drives.insert("V:\\");
        drives.insert("W:\\");
        drives.insert("X:\\");
        drives.insert("Y:\\");
        drives.insert("Z:\\");
        JSONNode array(JSON_ARRAY);
        array.set_name("data");
        for (std::set<std::string>::iterator d = drives.begin(); d != drives.end(); d++) {
          try {
            boost::filesystem::file_status fs = boost::filesystem::status(*d);

            if (boost::filesystem::status_known(fs) && boost::filesystem::exists(*d)) {
              buildFile((*d),(*d),true, array);
            }
          } catch (boost::filesystem::filesystem_error & er) {
            LOGINFO(er.what());
          }
        }
        result.push_back(array);
        result.setStatus(res.HTTP_OK, "");
      } else
#endif
        if (f.isDirectory()) {
        org::esb::io::FileList flist = f.listFiles();
        JSONNode array(JSON_ARRAY);
        array.set_name("data");

        foreach(org::esb::io::FileList::value_type &file, flist) {
          buildFile(*file.get(), array);
        }
        result.push_back(array);
        result.setStatus(res.HTTP_OK, "");
      } else if (f.isFile()) {
        JSONNode array(JSON_ARRAY);
        array.set_name("data");
        buildFile(f, array);
        result.push_back(array);
        result.setStatus(res.HTTP_OK, "");
      } else {
        result.setStatus(res.HTTP_NOT_FOUND, "file not found");
      }
    }
    //res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    
    ostr << result.write_formatted();
  }

  void buildFile(org::esb::io::File & file, JSONNode & node) {
    buildFile(file.getFilePath(),file.getFileName(),file.isDirectory(), node);
  }

  void buildFile(std::string path, std::string name, bool isDir, JSONNode & node) {
    LOGDEBUG("FileName:"<<name);
    JSONNode data(JSON_NODE);
    data.push_back(JSONNode("path", path));
    data.push_back(JSONNode("name", name));
    data.push_back(JSONNode("directory", isDir));
    node.push_back(data);
  }

};
REGISTER_WEB_HOOK("/api/v1/file/?([\\w-/:]+)", GET, FileHandler);



