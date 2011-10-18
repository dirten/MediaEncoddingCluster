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
#include <boost/thread.hpp>

namespace org {
  namespace esb {
    namespace core {

      class CORE_EXPORT HookNotificationCenter {
      public:
        HookNotificationCenter();
        virtual ~HookNotificationCenter();
        static  HookNotificationCenter * getInstance();
        void  postHook(std::string name,Request  * object1=NULL,Response * object2=NULL);
        void  addObserver(std::string name, boost::function<void (Request*, Response*)> func, int prio);
      private:
        typedef std::list<boost::function<void (Request*req, Response*res)> > FuncList;
        typedef std::map<int, FuncList > FuncMap;
        static HookNotificationCenter * _instance;
        std::map<std::string, FuncMap > _hook_map;
        
        boost::mutex hook_mutex;
      };
    }
  }
}

#endif	/* HOOKNOTIFICATIONCENTER_H */

