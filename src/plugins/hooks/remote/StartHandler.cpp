
#include "org/esb/grid/GridRegistry.h"
#include "org/esb/grid/GridNode.h"
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/core/PluginRegistry.h"
#include "org/esb/libjson/JSONResult.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/Foreach.h"
#include "Poco/URI.h"
#include "Poco/Net/NameValueCollection.h"
#include "RemoteController.h"
#include "org/esb/util/Foreach.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/URI.h"
#include "Poco/StreamCopier.h"
#include "Poco/StringTokenizer.h"
#include "Poco/URI.h"

using org::esb::core::PluginRegistry;
using org::esb::grid::GridRegistry;
using org::esb::grid::GridNode;

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
using Poco::URI;
namespace plugin{
  using org::esb::core::OptionsDescription;
  using org::esb::core::PluginRegistry;
  class StartHandler : public org::esb::core::WebHookPlugin {

    void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
      JSONResult result(req);
      std::string pluginname=req.get("pluginname");
      LOGDEBUG("starting plugin : "<<pluginname);



      OptionsDescription desc=PluginRegistry::getInstance()->getOptionsDescription(pluginname);

      std::vector<std::string> args;
      typedef boost::shared_ptr<boost::program_options::option_description> option;
      foreach(const option value, desc.options()) {
        if(req.has(value->long_name())){
          LOGINFO("plugin parameter found:"<<value->long_name());
          args.push_back("--"+value->long_name());
          args.push_back(req.get(value->long_name()));
        }else{
          LOGINFO("plugin parameter \""<<value->long_name()<<"\" not found");
        }
      }

      Poco::Net::NameValueCollection::ConstIterator it=req.begin();
      for(;it!=req.end();it++){
        LOGDEBUG("starting plugin param : "<<(*it).first<<" = "<<(*it).second);
      }

      RemoteController::startPlugin(pluginname, args);

      std::ostream& ostr = res.send();
      ostr << result.write_formatted();


    }
  };

  REGISTER_WEB_HOOK("/api/v1/remote/start/{pluginname}?(?:\\?.*)?$", POST, StartHandler)

  }
