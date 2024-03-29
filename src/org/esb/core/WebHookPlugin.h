/* 
 * File:   HookPlugin.h
 * Author: HoelscJ
 *
 * Created on 30. August 2011, 16:46
 */

#ifndef WEBHOOKPLUGIN_H
#define	WEBHOOKPLUGIN_H
#include "http/HTTPServerRequest.h"
#include "http/HTTPServerResponse.h"
#include "Plugin.h"
#include "PluginRegistry.h"
#include "HookNotificationCenter.h"

namespace org {
  namespace esb {
    namespace core {

      class WebHookPlugin : public org::esb::core::http::RequestHandler, public Plugin {
      public:
        //virtual void handle(org::esb::core::http::HTTPServerRequest& request, org::esb::core::http::HTTPServerResponse& response) = 0;

        virtual ~WebHookPlugin() {
        }
      private:

      };

    }
  }
}
#define GET "GET"
#define POST "POST"
#define PUT "PUT"
#ifdef DELETE
#undef DELETE
#endif
#define DELETE "DELETE"
#define HEAD "HEAD"
#define OPTIONS "OPTIONS"

//#define UNIQUE2(x,y) x##y
//#define UNIQUE1(x,y) UNIQUE2(x,y)
//#define UNIQUE(x) UNIQUE1(x,__COUNTER__)
//#define CLAZZ UNIQUE(Register##clazz) 
#define REGISTER_WEB_HOOK(url,method,clazz) \
  class Register##clazz : public org::esb::core::WebHookFactory \
{ \
  public: \
  Register##clazz(){ \
    org::esb::core::PluginRegistry::getInstance()->registerWebHookFactory(#clazz, this); \
  } \
  org::esb::core::WebHookPlugin* create() { \
    return static_cast<org::esb::core::WebHookPlugin*>(new clazz()); \
  } \
  org::esb::core::OptionsDescription getOptionsDescription() { \
    return Ptr<org::esb::core::WebHookPlugin>(new clazz())->getOptionsDescription(); \
  } \
  std::string getUrl(){return url;} \
  std::string getMethod(){return method;} \
  } Register##clazz##Instance;


#endif	/* HOOKPLUGIN_H */

