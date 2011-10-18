/* 
 * File:   HookNotificationCenter.cpp
 * Author: HoelscJ
 * 
 * Created on 31. August 2011, 11:34
 */

#include "HookNotificationCenter.h"
#include "Request.h"
#include "Response.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/Log.h"

namespace org {
  namespace esb {
    namespace core {
      HookNotificationCenter * HookNotificationCenter::_instance = NULL;

      HookNotificationCenter::HookNotificationCenter() {

      }

      HookNotificationCenter::~HookNotificationCenter() {

      }

      HookNotificationCenter * HookNotificationCenter::getInstance() {
        if (_instance == NULL)
          _instance = new HookNotificationCenter();
        //LOGDEBUG("NotificationCenter:" << _instance);
        return _instance;
      }

      void HookNotificationCenter::postHook(std::string name, Request * req, Response * res) {
        boost::mutex::scoped_lock enqueue_lock(hook_mutex);
        //LOGDEBUG("postHook " << name << " On " << this );
          foreach(FuncMap::value_type & data, _hook_map["*"]) {
            data.second(req, res);
          }

        if (_hook_map.count(name) > 0) {
          foreach(FuncMap::value_type & data, _hook_map[name]) {
            data.second(req, res);
          }
        }
      }

      void HookNotificationCenter::addObserver(std::string name, boost::function<void (Request*req, Response*res) > func, int prio) {
        LOGDEBUG( "AddObserver" << name << " On " << this );
        _hook_map[name][prio]=func;
      }
    }
  }
}
