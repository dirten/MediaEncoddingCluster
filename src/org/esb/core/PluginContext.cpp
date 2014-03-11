/* 
 * File:   PluginContext.cpp
 * Author: HoelscJ
 * 
 * Created on 4. Oktober 2011, 10:52
 */

//#include "org/esb/db/hivedb.hpp"

#include "PluginContext.h"
#include "org/esb/config/config.h"
#include "org/esb/core/AppContext.h"
#include "org/esb/hive/DatabaseService.h"

namespace org {
  namespace esb {
    namespace core {
      using org::esb::hive::DatabaseService;
      PluginContext::PluginContext():database(DatabaseService::getDatabase()) {
        //database = boost::shared_ptr<db::HiveDb>(new db::HiveDb("sqlite3", org::esb::config::Config::get("db.url")));
      }


      PluginContext::~PluginContext() {
        //LOGDEBUG("PluginContext::~PluginContext()")
        //delete database;
      }
      
      bool PluginContext::contains(std::string key) {
        return _props.count(key)>0;
      }
      
      void PluginContext::merge(Ptr<PluginContext> ctx){
        std::map<std::string, std::string>::iterator inenvit=ctx->env.begin();
        for(;inenvit!=ctx->env.end();inenvit++){
          if((*inenvit).first!="data"&&env.count((*inenvit).first)){
            env.erase((*inenvit).first);
          }
        }
        
        env.insert(ctx->env.begin(),ctx->env.end());
        //return;
        
        std::map<std::string, boost::any>::iterator propsit=ctx->_props.begin();
        for(;propsit!=ctx->_props.end();propsit++){
          if((*propsit).first!="data"&&_props.count((*propsit).first)){
            _props.erase((*propsit).first);
          }
        }
        _props.insert(ctx->_props.begin(),ctx->_props.end());
      }
      
      std::list<std::string> PluginContext::keys(){
        std::list<std::string> result;
        std::map<std::string, boost::any>::iterator prop_it=_props.begin();
        for(;prop_it!=_props.end();prop_it++){
          result.push_back((*prop_it).first);
        }        
        return result;
      }
      
      std::string PluginContext::toString(){
        std::string result;
        std::map<std::string, std::string>::iterator env_it=env.begin();
        result+="Environment:\n";
        for(;env_it!=env.end();env_it++){
          result+=(*env_it).first+":"+(*env_it).second+"\n";
        }
        result+="Properties:\n";
        std::map<std::string, boost::any>::iterator prop_it=_props.begin();
        for(;prop_it!=_props.end();prop_it++){
          result+=(*prop_it).first+":"+boost::any_cast<std::string>((*prop_it).second)+"\n";
        }

        return result;
      }
    }
  }
}
