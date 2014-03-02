#include <string>
#include <list>
#include "org/esb/grid/GridRegistry.h"
#include "org/esb/grid/GridNode.h"
#include "RemoteController.h"
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/JSONResult.h"
#include "org/esb/util/Log.h"
#include "org/esb/core/PluginRegistry.h"
#include "org/esb/util/Foreach.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/StreamCopier.h"
using org::esb::core::PluginRegistry;
using org::esb::grid::GridRegistry;
using org::esb::grid::GridNode;
using org::esb::core::OptionsDescription;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
namespace plugin{
  class StatusHandler : public org::esb::core::WebHookPlugin {

    void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
      JSONResult result(req);
      std::string pluginname;
      if(req.has("pluginname")){
        pluginname=req.get("pluginname");
      }


      std::list<std::string>plugin_names=PluginRegistry::getInstance()->getPluginNameList();
      JSONNode c(JSON_ARRAY);
      c.set_name("data");

      foreach(std::string & name , plugin_names){
        bool running=RemoteController::running(name);
        //bool running=PluginRegistry::getInstance()->isRunning(name);
        JSONNode entry(JSON_NODE);
        entry.push_back(JSONNode("name",name));
        entry.push_back(JSONNode("running",running));

        JSONNode config(JSON_ARRAY);
        config.set_name("parameter");

        OptionsDescription desc = PluginRegistry::getInstance()->getOptionsDescription(name);
        typedef boost::shared_ptr<boost::program_options::option_description> option;
        foreach(const option value, desc.options()) {
          boost::any data;
          value->semantic()->apply_default(data);
          JSONNode centry(JSON_NODE);
          centry.push_back(JSONNode("name",value->long_name()));
          if (data.type() == typeid (int)) {
            centry.push_back(JSONNode("type","int"));
            centry.push_back(JSONNode("default",boost::any_cast<int>(data)));
          } else if (data.type() == typeid (double)) {
            centry.push_back(JSONNode("type","double"));
            centry.push_back(JSONNode("default",boost::any_cast<double>(data)));
          } else if (data.type() == typeid (bool)) {
            centry.push_back(JSONNode("type","bool"));
            centry.push_back(JSONNode("default",boost::any_cast<bool>(data)));
          } else if (data.type() == typeid (std::string)){
            centry.push_back(JSONNode("type","string"));
            centry.push_back(JSONNode("default",boost::any_cast<std::string>(data)));
          }
          config.push_back(centry);
        }
        entry.push_back(config);

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
