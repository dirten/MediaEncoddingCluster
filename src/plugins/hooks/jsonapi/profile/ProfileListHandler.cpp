#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/core/PluginContext.h"

#include "org/esb/libjson/libjson.h"
#include "org/esb/libjson/JSONResult.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/util/Foreach.h"
#include "../exports.h"

#include "org/esb/core/StorageEngine.h"
#include "org/esb/db/Profile.h"


using org::esb::model::Profile;

class JSONAPI_EXPORT ProfileListHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {


    JSONResult result(req);
    JSONNode c(JSON_ARRAY);
    c.set_name("data");
    std::list<Profile> profiles=getContext()->getStorageEngine()->getProfileList();
    foreach(Profile profile, profiles) {
      JSONNode prnode(JSON_NODE);
      prnode.push_back(JSONNode("uuid", profile.uuid));
      prnode.push_back(JSONNode("name", profile.name));
      c.push_back(prnode);
    }
    /*
    db::HiveDb db=getContext()->database;//("sqlite3", req.get("db.url"));
    vector<db::Preset> presets = litesql::select<db::Preset > (db).orderBy(db::Preset::Name).all();
    foreach(db::Preset preset, presets) {
      JSONNode prnode(JSON_NODE);
      prnode.push_back(JSONNode("uuid", preset.uuid.value()));
      prnode.push_back(JSONNode("name", preset.name.value()));
      c.push_back(prnode);
    }*/
    result.setData(c);

    res.setContentType("application/json");
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }

};
REGISTER_WEB_HOOK("/api/v1/profile/?(?:\\?.*)?$", GET, ProfileListHandler)






