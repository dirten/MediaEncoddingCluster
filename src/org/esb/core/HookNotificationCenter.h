/* 
 * File:   HookNotificationCenter.h
 * Author: HoelscJ
 *
 * Created on 31. August 2011, 11:34
 */

#ifndef HOOKNOTIFICATIONCENTER_H
#define	HOOKNOTIFICATIONCENTER_H
#include <string>
#include <map>
#include <list>
#include <boost/function.hpp>
#include "Request.h"
#include "Response.h"
#include "exports.h"
namespace org {
  namespace esb {
    namespace core {

      class  HookNotificationCenter {
      public:
        HookNotificationCenter();
        virtual ~HookNotificationCenter();
        static CORE_EXPORT HookNotificationCenter * getInstance();
        void CORE_EXPORT postHook(std::string name,Request * object1=NULL,Response * object2=NULL);
        void CORE_EXPORT addObserver(std::string name, boost::function<void (Request*, Response*)> func);
        //void addObserver(std::string name, boost::function<void (org::esb::api::ServiceRequest*)> func);
      private:
        static HookNotificationCenter * _instance;
        std::map<std::string, std::list<boost::function<void (Request*req, Response*res)> > > _hook_map;
      };
    }
  }
}

#endif	/* HOOKNOTIFICATIONCENTER_H */
