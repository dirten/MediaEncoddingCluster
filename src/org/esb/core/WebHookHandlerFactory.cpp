/* 
 * File:   WebHookHandlerFactory.cpp
 * Author: HoelscJ
 * 
 * Created on 26. Januar 2012, 17:42
 */

#include "WebHookHandlerFactory.h"
#include "WebHookPlugin.h"
#include "org/esb/util/Foreach.h"
namespace org {
  namespace esb {
    namespace core {

      WebHookHandlerFactory::WebHookHandlerFactory() {
      }

      org::esb::core::http::RequestHandler * WebHookHandlerFactory::createHandler(org::esb::core::http::HTTPServerRequest&req) {
        //LOGDEBUG("WebHookHandlerFactory::createHandler");
        foreach(WebHookFactory * factory,_webhook_factory_list){
          WebHookPlugin * ptr=factory->create();
          //LOGDEBUG(ptr);
          //LOGDEBUG(static_cast<org::esb::core::http::RequestHandler *>(ptr));
          return static_cast<org::esb::core::http::RequestHandler *>(ptr);
        }
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
