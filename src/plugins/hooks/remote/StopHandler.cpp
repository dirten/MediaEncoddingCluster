
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/JSONResult.h"
#include "org/esb/util/Log.h"

#include "RemoteController.h"

namespace plugin{
  class StopHandler : public org::esb::core::WebHookPlugin {

      void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
        JSONResult result(req);
        std::string pluginname=req.get("pluginname");
        LOGDEBUG("stopping plugin : "<<pluginname);

        RemoteController::stopPlugin(pluginname);

        std::ostream& ostr = res.send();
        ostr << result.write_formatted();
      }

  };

  REGISTER_WEB_HOOK("/api/v1/remote/stop/{pluginname}$", POST, StopHandler)

}

