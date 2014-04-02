/* 
 * File:   PluginContext.h
 * Author: HoelscJ
 *
 * Created on 4. Oktober 2011, 10:52
 */

#ifndef PLUGINCONTEXT_H
#define	PLUGINCONTEXT_H
#include "org/esb/db/hivedb.hpp"
#include <map>
#include <list>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include "exports.h"

#include "org/esb/lang/Ptr.h"

namespace db{
  class HiveDb;
  }
namespace org {
  namespace esb {
    namespace core {

      class StorageEngine;
      class CORE_EXPORT PluginContext {
      public:
        //boost::shared_ptr<db::HiveDb> database;
        db::HiveDb database;

        StorageEngine *  getStorageEngine();
        template<typename T>
        T getEnvironment(std::string key){
          return boost::lexical_cast<T>(env[key]);
        }
        
        template<typename T>
        T get(std::string key){
          return boost::any_cast<T>(_props[key]);
          boost::any data;
          if(_props.count(key)>0){
            data = _props[key];
            std::cout<<"Key="<<key<<" type = "<<data.type().name()<<std::endl;
          }
          return boost::any_cast<T>(data);
        }
        template<typename T>
        T getProperty(std::string key){
          boost::any data;
          if(_props.count(key)>0){
            data = _props[key];
            std::cout<<"Key="<<key<<" type = "<<data.type().name()<<std::endl;
          }
          return boost::any_cast<T>(data);
        }

        template<typename T>
        void set(std::string key, T value){
          _props[key]=value;
        }

        template<typename T>
        void setProperty(std::string key, T value){
          _props[key]=value;
        }

        void merge(Ptr<PluginContext>);
        std::list<std::string> keys();
        std::string toString();
        virtual ~PluginContext();
        bool contains(std::string);
      private:
        std::map<std::string, boost::any> _props;
        friend class PluginRegistry;
        friend class WebHookHandlerFactory;
        PluginContext(StorageEngine * engine=NULL);
        StorageEngine * _storageEngine;
        std::map<std::string, std::string> env;
      };
      }
    }
  }

#endif	/* PLUGINCONTEXT_H */

