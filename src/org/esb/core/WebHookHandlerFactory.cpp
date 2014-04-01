/* 
 * File:   WebHookHandlerFactory.cpp
 * Author: HoelscJ
 * 
 * Created on 26. Januar 2012, 17:42
 */

#include "WebHookHandlerFactory.h"
#include "WebHookPlugin.h"
#include "WebHookProxy.h"
#include "PluginContext.h"
#include "http/RootRequestHandler.h"
#include "org/esb/util/Foreach.h"
#include "Poco/RegularExpression.h"
#include "Poco/StringTokenizer.h"
#include "Poco/URI.h"
#include "org/esb/util/UUID.h"
#include "org/esb/config/config.h"
#include "org/esb/util/StringUtil.h"
namespace org {
  namespace esb {
    namespace core {
using namespace Poco;
      WebHookHandlerFactory::WebHookHandlerFactory() {
        _user_path = org::esb::config::Config::get("hive.graph_path");

      }

      org::esb::core::http::RequestHandler * WebHookHandlerFactory::createHandler(org::esb::core::http::HTTPServerRequest&req) {
        //LOGDEBUG("WebHookHandlerFactory::createHandler");
        /*only process the plugins when it is an api call*/
        if (req.getURI().find("/api/v1/") != string::npos) {

          /*here is the right place to intercept proxy requests for remote hosts in the case that is an "ip" request parameter present*/
          std::string param=URI(req.getURI()).getQuery();
          Poco::StringTokenizer param_tokenz(param,"&");
          LOGDEBUG("query parameter : "<<param);
          bool use_proxy=false;
          for(Poco::StringTokenizer::Iterator it=param_tokenz.begin();it!=param_tokenz.end();it++){
            Poco::StringTokenizer key_tokenz(*it,"=");
            if(key_tokenz[0]=="ip"){
              use_proxy=true;
            }
          }
          if(use_proxy){
            LOGDEBUG("redirect to proxy")
            return static_cast<org::esb::core::http::RequestHandler *> (new WebHookProxy());
          }
          LOGDEBUG("WebHookFactory.size()" << _webhook_factory_list.size());
          foreach(WebHookFactory * factory, _webhook_factory_list) {
            std::string uri=req.getURI();
            //uri=StringUtil::replace(uri,".","\\.");
            //uri=Poco::replace(uri, ".", "\\.");
            LOGDEBUG("RegEx="+factory->getUrl() << " / uri=" << uri);
            LOGDEBUG("Request Method"+factory->getMethod()<<" / "<< req.getMethod());
            if (factory->getMethod() == req.getMethod()) {
              /*matching url paceholder*/
              std::string url = factory->getUrl();
              Poco::StringTokenizer tokenz(uri,"?");
              //uri=tokenz[0];
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
                    url = Poco::replace(url, needle, std::string("([\\w-\\.]+)"));
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
              if (re.match(uri, 0, posVec2)) {
                LOGDEBUG("found:" << posVec2.size());
                for (int a = 1; a < posVec2.size(); a++) {
                  LOGDEBUG("setting parameter a=" << a << " : " << varVec[a] << "=" << uri.substr(posVec2[a].offset, posVec2[a].length));
                  req.add(varVec[a], uri.substr(posVec2[a].offset, posVec2[a].length));
                  req.add(org::esb::util::StringUtil::toString(a), uri.substr(posVec2[a].offset, posVec2[a].length));
                }
                /*
                if(var.length()){
                  if(posVec2.size()==2){
                    LOGDEBUG("setting parameter "<<var<<"="<<req.getURI().substr(posVec2[1].offset,posVec2[1].length));
                    req.add(var,req.getURI().substr(posVec2[1].offset,posVec2[1].length));
                  }
                }*/

                /*adding get parameter to the request*/
                std::string param=Poco::URI(req.getURI()).getQuery();
                Poco::StringTokenizer param_tokenz(param,"&");
                LOGDEBUG("parameter : "<<param);
                for(Poco::StringTokenizer::Iterator it=param_tokenz.begin();it!=param_tokenz.end();it++){
                  Poco::StringTokenizer key_tokenz(*it,"=");

                  LOGDEBUG("setting query parameter -> key="<<key_tokenz[0]<<" val="<< (key_tokenz.count()>1?key_tokenz[1]:""));
                  req.add(key_tokenz[0], key_tokenz.count()>1?key_tokenz[1]:"");

                }
                req.add("requestUUID", org::esb::util::PUUID());
                req.add("hive.graph_path", _user_path);
                req.add("db.url", org::esb::config::Config::get("db.url"));
                req.add("hive.tmp_path", org::esb::config::Config::get("hive.tmp_path"));
                WebHookPlugin * ptr = factory->create();
                ptr->setContext(new PluginContext());
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
