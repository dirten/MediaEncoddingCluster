/* 
 * File:   WebHookHandlerFactory.cpp
 * Author: HoelscJ
 * 
 * Created on 26. Januar 2012, 17:42
 */

#include "WebHookHandlerFactory.h"
#include "org/esb/util/Foreach.h"
namespace org {
  namespace esb {
    namespace core {

      WebHookHandlerFactory::WebHookHandlerFactory() {
      }

      org::esb::core::http::RequestHandler * WebHookHandlerFactory::createHandler(org::esb::core::http::HTTPServerRequest&req) {
        foreach(WebHookFactory * factory,_webhook_factory_list){
          return NULL;//factory->create().get();
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
