
#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/core/PluginContext.h"

#include "org/esb/libjson/libjson.h"
#include "org/esb/libjson/JSONResult.h"
#include "../exports.h"

#include "org/esb/core/StorageEngine.h"
#include "org/esb/db/Profile.h"

using org::esb::model::Profile;

class JSONAPI_EXPORT ProfileDeleteHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    JSONResult result(req);
    std::string id = req.get("profileid");

    Profile profile=getContext()->getStorageEngine()->getProfileByUUID(id);
    if (profile.uuid==id) {
      if(getContext()->getStorageEngine()->deleteProfile(profile)){
        result.setStatus("ok","profile successful deleted");
      }else{
        result.setStatus(res.HTTP_NOT_FOUND, "profile not found");
      }
    }

    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }
};
REGISTER_WEB_HOOK("/api/v1/profile/{profileid}$", DELETE, ProfileDeleteHandler)
