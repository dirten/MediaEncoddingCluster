/* 
 * File:   GraphVerifier.cpp
 * Author: HoelscJ
 * 
 * Created on 6. Dezember 2011, 13:08
 */

#include "GraphVerifier.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/lang/Ptr.h"
#include "org/esb/core/Task.h"
namespace graph {
  GraphVerifier * GraphVerifier::_instance = 0;

  GraphVerifier * GraphVerifier::getInstance() {
    if (_instance == 0)
      _instance = new GraphVerifier();
    return _instance;
  }

  bool GraphVerifier::verifyTasks(JSONNode& node) {
    LOGDEBUG("bool GraphVerifier::verifyTasks(JSONNode& node)");
    bool result = true;
    if(node.contains("tasks")){
      int s = node["tasks"].size();
      LOGDEBUG("Array Size=" << s);
      for (int a = 0; a < s; a++) {
        JSONNode task=node["tasks"].at(a);
        if(!verifyTask(task)){
          result=false;
          _status="error";
          break;
        }
      }
    }else{
      _status="error";
      _message="there is not tasks array defined in json";
      result = false;
    }
    return result;
  }

  bool GraphVerifier::verifyTask(JSONNode& node) {
    bool result = false;
    //std::string result = "";
    if (!node.contains("name")) {
      _message = "one task has no defined name";
      _status="error";
    } else {
      /*first create an empty named task to resolve the required parameter for it*/
      Ptr<org::esb::core::Task>task = org::esb::core::PluginRegistry::getInstance()->createTask(node["name"].as_string(), std::map<std::string, std::string>());
      if (!task) {
        _message = std::string("could not find a definition for task with name ").append(node["name"].as_string());
      } else {
        org::esb::core::OptionsDescription desc = task->getOptionsDescription();
        std::map<std::string, std::string> para;
        typedef boost::shared_ptr<boost::program_options::option_description> option;
        std::string parameter;

        foreach(const option value, desc.options()) {
          std::string def;
          if (!value->semantic()->is_required()) {
            boost::any data;
            value->semantic()->apply_default(data);
            if (data.type() == typeid (int)) {
              def = org::esb::util::StringUtil::toString(boost::any_cast<int>(data));
            } else if (data.type() == typeid (double)) {
              def = org::esb::util::StringUtil::toString(boost::any_cast<double>(data));
            } else if (data.type() == typeid (bool)) {
              def = org::esb::util::StringUtil::toString(boost::any_cast<bool>(data));
            } else if (data.type() == typeid (std::string)) {
              def = org::esb::util::StringUtil::toString(boost::any_cast<std::string > (data));
            } else {
              LOGERROR("Key=" << value->long_name() << " : Unknown value type:" << data.type().name());
            }
            LOGDEBUG("Key=" << value->long_name() << " Default=" << def);
          }
          if (!node.contains(value->long_name()) && (value->semantic()->is_required() || def.length() == 0)) {
            _message = std::string("attribute ").append(value->long_name()).append(" for Task ").append(node["name"].as_string()).append(" could not be empty!");
            break;
          }
          std::string key = value->long_name();
          std::string v;
          if(!node.contains(value->long_name())){
            v=def;
          }else{
                v = node[key].as_string();
          }
          parameter += key + "=" + v + ";";
          para[key]=v;
        }
        task = org::esb::core::PluginRegistry::getInstance()->createTask(node["name"].as_string(), para);
        //task->getContext()->_props["job"]=job;
        task->prepare();
        if(task->getStatus()==org::esb::core::Task::ERROR){
          _message=node["name"].as_string();
          _message+=":";
          _message+=task->getStatusMessage();
        }else{
          result=true;
        }
        LOGDEBUG("Task Found: " << node["name"].as_string());
      }
    }
    //return result;


    return result;
  }

  JSONNode GraphVerifier::getResult() {
    JSONNode result(JSON_NODE);
    result.push_back(JSONNode("status", _status));
    result.push_back(JSONNode("message", _message));
    return result;
  }

  GraphVerifier::GraphVerifier() {
    _status = "ok";
  }

  GraphVerifier::~GraphVerifier() {
  }
}
