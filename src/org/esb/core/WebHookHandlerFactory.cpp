/* 
 * File:   WebHookHandlerFactory.cpp
 * Author: HoelscJ
 * 
 * Created on 26. Januar 2012, 17:42
 */

#include "WebHookHandlerFactory.h"
#include "WebHookPlugin.h"
#include "http/RootRequestHandler.h"
#include "org/esb/util/Foreach.h"
#include "Poco/RegularExpression.h"
#include "org/esb/util/UUID.h"
#include "org/esb/config/config.h"
#include "org/esb/util/StringUtil.h"
namespace org {
  namespace esb {
    namespace core {

      WebHookHandlerFactory::WebHookHandlerFactory() {
        _user_path = org::esb::config::Config::get("hive.graph_path");

      }

      org::esb::core::http::RequestHandler * WebHookHandlerFactory::createHandler(org::esb::core::http::HTTPServerRequest&req) {
        //LOGDEBUG("WebHookHandlerFactory::createHandler");
        /*only process the plugins when it is an api call*/
        if (req.getURI().find("/api/v1/") != string::npos) {
          LOGDEBUG("WebHookFactory.size()" << _webhook_factory_list.size());
          foreach(WebHookFactory * factory, _webhook_factory_list) {
            LOGDEBUG(factory->getUrl() << " / " << req.getURI());
            if (factory->getMethod() == req.getMethod()) {
              /*matching url paceholder*/
              std::string url = factory->getUrl();
              Poco::RegularExpression reholder("\\{(.*?)\\}");
              Poco::RegularExpression::MatchVec posVec;
              //std::string var;
              std::map<int, std::string> varVec;
              int round = 1;
              //int maxround = 0;
              while (int r = reholder.match(url, 0, posVec)) {
                LOGDEBUG("posVec:" << posVec.size());
                LOGDEBUG("r=" << r);
                for (int a = 1; a < posVec.size(); a++) {
                  varVec[round] = url.substr(posVec[a].offset, posVec[a].length);
                  std::string needle = std::string("{") + varVec[round] + "}";
                  LOGDEBUG("substr:" << needle);
                  if (round == 1)
                    url = Poco::replace(url, needle, std::string("([\\w-]+)"));
                  else
                    url = Poco::replace(url, needle, std::string("(/?.+)"));
                  LOGDEBUG("new Url:" << url);
                  ++round;
                }
              }
              if (round == 3)
                url = Poco::replace(url, "([\\w-]+)", "(/?.+)");
              //url += "/?$";
              LOGDEBUG("match url:"<<url);
              Poco::RegularExpression re(url);
              //LOGDEBUG(factory->getUrl()<<" / "<<req.getURI());
              Poco::RegularExpression::MatchVec posVec2;
              if (re.match(req.getURI(), 0, posVec2)) {
                LOGDEBUG("found:" << posVec2.size());
                for (int a = 1; a < posVec2.size(); a++) {
                  LOGDEBUG("setting parameter a=" << a << " : " << varVec[a] << "=" << req.getURI().substr(posVec2[a].offset, posVec2[a].length));
                  req.add(varVec[a], req.getURI().substr(posVec2[a].offset, posVec2[a].length));
                  req.add(org::esb::util::StringUtil::toString(a), req.getURI().substr(posVec2[a].offset, posVec2[a].length));
                }
                /*
                if(var.length()){
                  if(posVec2.size()==2){
                    LOGDEBUG("setting parameter "<<var<<"="<<req.getURI().substr(posVec2[1].offset,posVec2[1].length));
                    req.add(var,req.getURI().substr(posVec2[1].offset,posVec2[1].length));
                  }
                }*/
                req.add("requestUUID", org::esb::util::PUUID());
                req.add("hive.graph_path", _user_path);
                req.add("db.url", org::esb::config::Config::get("db.url"));
                req.add("hive.tmp_path", org::esb::config::Config::get("hive.tmp_path"));
                WebHookPlugin * ptr = factory->create();
                return static_cast<org::esb::core::http::RequestHandler *> (ptr);
              }
            }
          }

        }
        return new http::RootRequestHandler();
      }

      void WebHookHandlerFactory::registerHandlerFactory(WebHookFactory *factory) {
        _webhook_factory_list.push_back(factory);
        //_webhook_plugin_list[factory->getUrl()]=factory->create();
      }

      WebHookHandlerFactory::~WebHookHandlerFactory() {
      }
    }
  }
}
