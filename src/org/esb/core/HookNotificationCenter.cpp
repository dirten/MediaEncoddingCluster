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
        return _instance;
      }

      void HookNotificationCenter::postHook(std::string name, Request * req, Response * res) {
        if(_hook_map.count(name)>0){
          foreach(boost::function<void (Request*req, Response*res)> func,_hook_map[name]){
            func(req,res);
          }
        }
      }

      void HookNotificationCenter::addObserver(std::string name, boost::function<void (Request*req, Response*res)> func) {
        _hook_map[name].push_back(func);
      }
    }
  }
}
