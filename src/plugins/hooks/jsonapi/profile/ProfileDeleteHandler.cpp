
#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/libjson/JSONResult.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/util/Foreach.h"
#include "../exports.h"

class JSONAPI_EXPORT ProfileDeleteHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    JSONResult result(req);
    db::HiveDb db("sqlite3", req.get("db.url"));
    std::string id = req.get("profileid");
    litesql::DataSource<db::Preset>s = litesql::select<db::Preset > (db, db::Preset::Uuid == id);
    if (s.count() > 0) {
      db::Preset preset = s.one();
      preset.del();
      result.setStatus("ok","profile successful deleted");
    } else {
      result.setStatus(res.HTTP_NOT_FOUND, "profile not found");
    }
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }
};
REGISTER_WEB_HOOK("/api/v1/profile/{profileid}$", DELETE, ProfileDeleteHandler);
