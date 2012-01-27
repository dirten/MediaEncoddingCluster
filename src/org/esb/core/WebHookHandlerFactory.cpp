/* 
 * File:   WebHookHandlerFactory.cpp
 * Author: HoelscJ
 * 
 * Created on 26. Januar 2012, 17:42
 */

#include "WebHookHandlerFactory.h"
#include "WebHookPlugin.h"
#include "org/esb/util/Foreach.h"
#include "Poco/RegularExpression.h"
namespace org {
  namespace esb {
    namespace core {

      WebHookHandlerFactory::WebHookHandlerFactory() {
      }

      org::esb::core::http::RequestHandler * WebHookHandlerFactory::createHandler(org::esb::core::http::HTTPServerRequest&req) {
        //LOGDEBUG("WebHookHandlerFactory::createHandler");
        foreach(WebHookFactory * factory,_webhook_factory_list){
          LOGDEBUG(factory->getUrl()<<" / "<<req.getURI());
          if(factory->getMethod()==req.getMethod()){
            /*matching url paceholder*/
            std::string url=factory->getUrl();
            Poco::RegularExpression reholder(".*\\{(.*)\\}.*");
            Poco::RegularExpression::MatchVec posVec;
            std::string var;
            if(reholder.match(url,0,posVec)){
              LOGDEBUG("posVec:"<<posVec.size());
              if(posVec.size()==2){
                var=url.substr(posVec[1].offset,posVec[1].length);
                LOGDEBUG("substr:"<<url.substr(posVec[1].offset,posVec[1].length));
                std::string needle=std::string("{")+url.substr(posVec[1].offset,posVec[1].length)+"}";
                url=Poco::replace(url,needle,std::string("(.*)"));
                LOGDEBUG("new Url:"<<url);
              }
              
            }
            Poco::RegularExpression re(url);
            //LOGDEBUG(factory->getUrl()<<" / "<<req.getURI());
            Poco::RegularExpression::MatchVec posVec2;
            if(re.match(req.getURI(),0,posVec2)){
              LOGDEBUG("found:"<<posVec2.size());
              if(var.length()){
                if(posVec2.size()==2){
                  LOGDEBUG("setting parameter "<<var<<"="<<req.getURI().substr(posVec2[1].offset,posVec2[1].length));
                  req.add(var,req.getURI().substr(posVec2[1].offset,posVec2[1].length));
                }
              }
              WebHookPlugin * ptr=factory->create();
              return static_cast<org::esb::core::http::RequestHandler *>(ptr);
            }
          }
        }
        return NULL;
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
