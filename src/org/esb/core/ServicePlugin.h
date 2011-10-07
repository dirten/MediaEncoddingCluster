/* 
 * File:   ServicePlugin.h
 * Author: HoelscJ
 *
 * Created on 29. August 2011, 15:48
 */

#ifndef SERVICEPLUGIN_H
#define	SERVICEPLUGIN_H
#include "Plugin.h"
#include "PluginRegistry.h"
namespace org {
  namespace esb {
    namespace core {

      class ServicePlugin: public org::esb::core::Plugin{
      public:
        enum ServiceType{
          SERVICE_TYPE_ALL,
          SERVICE_TYPE_SERVER,
          SERVICE_TYPE_CLIENT,
        };
        virtual void startService()=0;
        virtual void stopService()=0;
        virtual ServiceType getServiceType(){
          return SERVICE_TYPE_ALL;
        }
        virtual ~ServicePlugin(){};
      private:

      };
    }
  }
}
#define REGISTER_SERVICE(name,type) \
	class Register##type \
	        { \
                        org::esb::core::ServicePlugin* element##type; \
	                public: \
	                        Register##type() \
	                        { \
                                element##type=new type(); \
	                        org::esb::core::PluginRegistry::getInstance()->registerService(std::string(name), element##type); \
	                        } \
                                ~Register##type(){ \
                                delete element##type; \
                                } \
	        } Register##type##Instance; 

#endif	/* SERVICEPLUGIN_H */

