#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/CountingStream.h"
#include "Poco/StreamCopier.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/util/Foreach.h"
#include "../JSONResult.h"
#include "../PresetVerifier.h"
#include "../exports.h"

class JSONAPI_EXPORT ProfileUpdateHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    JSONResult result;
    result.push_back(JSONNode("requestUUID", req.get("requestUUID")));
    if (req.getContentLength() > 1024 * 1024) {
      result.setStatus("error", "Post data to big, maximum allowed size is 1024KB");
    } else {
      std::string data;
      Poco::StreamCopier::copyToString(req.stream(), data);
      try {
        if (libjson::is_valid(data)) {
          JSONNode inode = libjson::parse(data);
          std::string msg = checkJsonProfile(inode);
          if (msg.length() > 0) {
            result.setStatus("error", msg);
          } else {
            db::HiveDb db("sqlite3", req.get("db.url"));
            std::string iddata = req.get("profileid");
            litesql::DataSource<db::Preset>s = litesql::select<db::Preset > (db, db::Preset::Uuid == iddata);
            if (s.count() == 1) {
              LOGDEBUG("Update profile");
              db::Preset preset = s.one();
              preset.data = data;
              preset.name = inode["name"].as_string();
              preset.update();
              result.push_back(JSONNode("id", iddata));
            } else {
              result.setStatus("error", "profile with the given id not found");
              res.setStatusAndReason(res.HTTP_NOT_FOUND, "profile with the given id not found");
            }
          }
        } else {
          result.setStatus("error", "no valid json format given");
        }
      } catch (std::exception &ex) {
        result.setStatus("error", ex.what());
      }
    }
    res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }

  std::string checkJsonProfile(JSONNode&root) {
    std::string result;
    /*check the root conatins required data*/
    if (!root.contains("name")) {
      result = "no profile name given!";
    } else
      if (!root.contains("format")) {
      result = "no format attribute found!";
    } else
      if (!root.contains("video")) {
      result = "no video attribute found!";
    } else
      if (!root.contains("audio")) {
      result = "no audio attribute found!";
    } else
      if (!root["format"].contains("id")) {
      result = "no id attribute found in attribute \"format\"!";
    } else
      if (!root["video"].contains("id")) {
      result = "no id attribute found in attribute \"video\"!";
    } else
      if (!root["audio"].contains("id")) {
      result = "no id attribute found in attribute \"audio\"!";
    }else
      result = org::esb::plugin::PresetVerifier::verify(root);
    return result;
  }

};
REGISTER_WEB_HOOK("/api/v1/profile/{profileid}$", PUT, ProfileUpdateHandler);

