#include <string>
#include <list>
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/JSONResult.h"
#include "org/esb/util/Log.h"
#include "org/esb/core/PluginRegistry.h"
#include "RemoteController.h"
#include "org/esb/util/Foreach.h"
using org::esb::core::PluginRegistry;

namespace plugin{
  class StatusHandler : public org::esb::core::WebHookPlugin {

      void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
        JSONResult result(req);
        std::string pluginname;
        if(req.has("pluginname")){
          pluginname=req.get("pluginname");
        }

        /*if it has an ip as parameter, then make a remote call like a transparent proxy*/
        if(req.has("ip")){
          LOGDEBUG("ip="<<req.get("ip"))
        }

        std::list<std::string>plugin_names=PluginRegistry::getInstance()->getPluginNameList();
          JSONNode c(JSON_ARRAY);
          c.set_name("data");

          foreach(std::string & name , plugin_names){
            bool running=RemoteController::running(name);
            JSONNode entry(JSON_NODE);
            entry.push_back(JSONNode("name",name));
            entry.push_back(JSONNode("running",running));
            if(pluginname.empty() || pluginname==name)
              c.push_back(entry);
          }
          result.setData(c);

        std::ostream& ostr = res.send();
        ostr << result.write_formatted();
      }

  };
  typedef StatusHandler ServiceListHandler;

  REGISTER_WEB_HOOK("/api/v1/remote/status/?(?:\\?.*)?$", GET, ServiceListHandler)
  //REGISTER_WEB_HOOK("/api/v1/remote/status/{pluginname}$", GET, StatusHandler)

}
