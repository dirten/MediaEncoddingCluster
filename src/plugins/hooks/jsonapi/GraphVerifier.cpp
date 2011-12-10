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
  
  bool GraphVerifier::verify(JSONNode& node, boost::shared_ptr<db::HiveDb> db) {
    bool result=true;
    _db=db;   
    result=verifyTasks(node);
    if(result==true)
      result=verifyLinks(node);
    if(result==true)
      result=verifyCycleRedundance(node);
    return result;
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
      _message="there is no tasks array defined in json";
      result = false;
    }
    return result;
  }

  bool GraphVerifier::verifyLinks(JSONNode& node) {
    LOGDEBUG("bool GraphVerifier::verifyLinks(JSONNode& node)");
    bool result = true;
    if(node.contains("links")){
      int s = node["links"].size();
      for (int a = 0; a < s; a++) {
        JSONNode link=node["links"].at(a);
        if(!verifyLink(link)){
          result=false;
          _status="error";
          break;
        }
      }
    }else{
      _status="error";
      _message="there is no links array defined in json";
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
        typedef boost::shared_ptr<boost::program_options::option_description> Option;
        std::string parameter;

        foreach(const Option value, desc.options()) {
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
          if (!node.contains(value->long_name()) && (value->semantic()->is_required())) {
            _message = std::string("attribute ").append(value->long_name()).append(" for Task ").append(node["name"].as_string()).append(" could not be empty!");
            LOGERROR(_message);
            break;
          }
          std::string key = value->long_name();
          std::string v;
          LOGDEBUG("Looking for key:"<<value->long_name()<<" in node");
          if(!node.contains(value->long_name())){
            LOGDEBUG("Key not found");
            v=def;
          }else{
            LOGDEBUG("Key found");
            
            v = node[key].write_formatted();
          }
          parameter += key + "=" + v + ";";
          para[key]=v;
          LOGDEBUG("Parameter:"<<parameter);
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
      if(node.contains("uid")){
        nodes[node["uid"].as_string()]=node;
      }
    }
    //return result;

    
    return result;
  }
  
  bool GraphVerifier::verifyLink(JSONNode& node) {
    bool result=true;
    if(node.contains("uid")){
      if(nodes.count(node["uid"].as_string())){
        if(node.contains("linksTo")){
          if(nodes.count(node["linksTo"].as_string())){
            /*if(linked_nodes.count(node["linksTo"].as_string())){
              result=false;
              _status="error";
              _message="cyclic redundancy found";
            }else*/
            linked_nodes.insert(node["uid"].as_string());
          }else{
          result=false;
          _status="error";
          _message="target node does not exist in node list";            
          }
        }else{
          result=false;
          _status="error";
          _message="node does not link to a target node";
        }         
      }else{
        result=false;
        _status="error";
        _message="node could not be found in the list";
      }
    }else{
        result=false;
        _status="error";
        _message="node have no uid";
      }
    return result;
  }
  
  bool GraphVerifier::verifyCycleRedundance(JSONNode& node) {
    bool result=true;
    
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
