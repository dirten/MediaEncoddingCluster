/* 
 * File:   WebHookHandlerFactory.h
 * Author: HoelscJ
 *
 * Created on 26. Januar 2012, 17:42
 */

#ifndef WEBHOOKHANDLERFACTORY_H
#define	WEBHOOKHANDLERFACTORY_H
#include "http/RequestHandlerFactory.h"
#include "http/HTTPServerRequest.h"
#include "http/HTTPServerResponse.h"
#include "Plugin.h"
#include <list>
#include <map>
namespace org {
  namespace esb {
    namespace core {

      class WebHookHandlerFactory : public org::esb::core::http::RequestHandlerFactory {
      public:
        WebHookHandlerFactory();
        virtual ~WebHookHandlerFactory();
        org::esb::core::http::RequestHandler * createHandler(org::esb::core::http::HTTPServerRequest&req);
        void registerHandlerFactory(WebHookFactory *factory);
      private:
        std::list<WebHookFactory*> _webhook_factory_list;
        typedef std::map<std::string, org::esb::lang::Ptr<WebHookPlugin> > WebHookPluginMap;
         WebHookPluginMap _webhook_plugin_list;
      };
    }
  }
}

#endif	/* WEBHOOKHANDLERFACTORY_H */

