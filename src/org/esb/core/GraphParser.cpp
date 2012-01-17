/* 
 * File:   GraphParser.cpp
 * Author: jhoelscher
 * 
 * Created on 12. Dezember 2011, 10:34
 */

#include <list>

#include "GraphParser.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/StringUtil.h"
#include "Graph.h"
#include "TaskException.h"
namespace org {
  namespace esb {
    namespace core {

      struct ElementWrapper {
        Graph::Element element;
        bool visited;
        bool finished;
      };

      GraphParser::GraphParser(std::string graphdata)throw (GraphException) {
        //        try {
        if (libjson::is_valid(graphdata)) {
          JSONNode node = libjson::parse(graphdata);
          if (node.contains("name")) {
            name = node["name"].as_string();
          }
          /*parsing tasks from graph*/
          if (node.contains("tasks")) {
            int s = node["tasks"].size();
            for (int a = 0; a < s; a++) {
              JSONNode task = node["tasks"].at(a);
              parseTask(task);
            }
          } else {
            throw GraphException("no tasks are defined in the graph");
          }

          /*parsing links from graph*/
          if (node.contains("links")) {
            int s = node["links"].size();
            for (int a = 0; a < s; a++) {
              JSONNode link = node["links"].at(a);
              parseLink(link);
            }
          } else {
            throw GraphException("no links are defined in the graph");
          }
          _baseNode = node;
          return;
          verifyLinks();
          verifyCycle();
        } else {
          throw GraphException("no valid json");
        }
        //        } catch (std::exception & ex) {
        //    	    LOGERROR(ex.what());
        //          throw GraphException(ex.what());
        //        }
      }

      GraphParser::~GraphParser() {
        elements.clear();
      }

      std::string GraphParser::getName() {
        return name;
      }

      std::string GraphParser::getInfile() {
        return infile;
      }

      void GraphParser::setInfile(std::string infile) {
        /*parsing tasks from graph*/
        if (_baseNode.contains("tasks")) {
          int s = _baseNode["tasks"].size();
          for (int a = 0; a < s; a++) {
            JSONNode & task = _baseNode["tasks"].at(a);
            if (task.contains("name") && task["name"].as_string() == "InputTask") {
              if (task.contains("data") && task["data"].contains("infile")){
                LOGDEBUG("setting infile for graph to:"<<infile);
                JSONNode tmp=JSONNode("infile", infile);
                task["data"]["infile"].swap(tmp);
                
                LOGDEBUG(task["data"].write_formatted());
              }
            }
          }
        } else {
          throw GraphException("no tasks are defined in the graph");
        }

      }
      
      std::string GraphParser::getGraphString() {
        return _baseNode.write_formatted();
      }

      void GraphParser::parse(JSONNode&) {

      }

      void GraphParser::parseTask(JSONNode& node) {
        if (!node.contains("name")) {
          throw GraphException("task is not named");
        } else {
          if (!node.contains("uid")) {
            throw GraphException("no uid for element");
          }
          if (node.contains("name") && node["name"].as_string() == "InputTask") {
            if (node.contains("data") && node["data"].contains("infile"))
              infile = node["data"]["infile"].as_string();
          }
          /*first create an empty named task to resolve the required parameter for it*/
          Ptr<org::esb::core::Task>task = org::esb::core::PluginRegistry::getInstance()->createTask(node["name"].as_string(), std::map<std::string, std::string > ());
          if (!task) {
            throw GraphException(std::string("could not find a definition for task with name ").append(node["name"].as_string()));
          } else {
            try {
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
                  std::string message = std::string("attribute ").append(value->long_name()).append(" for Task ").append(node["name"].as_string()).append(" could not be empty!");
                  LOGERROR(message);
                  //break;
                }
                std::string key = value->long_name();
                std::string v;
                LOGDEBUG("Looking for key:" << value->long_name() << " in node");
                if (!node.contains(value->long_name())) {
                  LOGDEBUG("Key not found");
                  v = def;
                } else {
                  LOGDEBUG("Key found");

                  v = node[key].write_formatted();
                }
                parameter += key + "=" + v + ";";
                para[key] = v;
              }

              LOGDEBUG("Parameter:" << parameter);
              task = org::esb::core::PluginRegistry::getInstance()->createTask(node["name"].as_string(), para);
              //task->getContext()->_props["job"]=job;
              task->prepare();
              if (task->getStatus() == org::esb::core::Task::ERROR) {
                throw GraphException("task prepare results in an error");
              }
              elements[node["uid"].as_string()] = new Graph::Element();
              elements[node["uid"].as_string()]->setTask(task);
              elements[node["uid"].as_string()]->parameter = parameter;
              elements[node["uid"].as_string()]->name = node["name"].as_string();
              elements[node["uid"].as_string()]->id = node["uid"].as_string();
              LOGDEBUG("Task Found: " << node["name"].as_string());
            } catch (org::esb::core::TaskException & ex) {
              throw GraphException(ex.displayText(), node["uid"].as_string());
            } catch (std::exception & ex) {
              throw GraphException(ex.what(), node["uid"].as_string());
            } catch (...) {
              throw GraphException("unknown error", node["uid"].as_string());
            }

          }
        }
      }

      void GraphParser::parseLink(JSONNode&node) {
        if (node.contains("uid")) {
          if (elements.count(node["uid"].as_string())) {
            if (node.contains("linksTo")) {
              if (elements.count(node["linksTo"].as_string())) {
                Ptr<Graph::Element> src_element = elements[node["uid"].as_string()];
                Ptr<Graph::Element> sink_element = elements[node["linksTo"].as_string()];
                LOGDEBUG("link " << src_element << ":" << src_element->id << " to " << sink_element << ":" << sink_element->id);
                //elements[node["uid"].as_string()].linksTo.push_back(elements[node["linksTo"].as_string()]);
                //elements[node["linksTo"].as_string()].dependsOn.push_back(elements[node["uid"].as_string()]);
                src_element->addChild(sink_element);
                sink_element->addParent(src_element);
              } else {
                throw GraphException("target node does not exist in node list");
              }
            } else {
              throw GraphException("node does not link to a target node");
            }
          } else {
            throw GraphException("node could not be found in the list");
          }
        } else {
          throw GraphException("link have no uid");
        }

      }

      void GraphParser::verifyLinks() {

        foreach(ElementMap::value_type & el, elements) {
          if (el.second->task->getPadTypes() & Task::SOURCE) {
            if (el.second->getParents().size() == 0) {
              throw GraphException(std::string("no sources configured for Element:") + el.first);
            }
          }
          if (el.second->task->getPadTypes() & Task::SINK) {
            if (el.second->getChilds().size() == 0) {
              throw GraphException(std::string("no sinks configured for Element:") + el.first);
            }
          }
          if (!(el.second->task->getPadTypes() & Task::SINK)) {
            if (el.second->getChilds().size() > 0) {
              throw GraphException(std::string("Element have no sink pad:") + el.first);
            }
          }
          if (!(el.second->task->getPadTypes() & Task::SOURCE)) {
            if (el.second->getParents().size() > 0) {
              throw GraphException(std::string("Element have no source pad:") + el.first);
            }
          }
        }
      }

      void GraphParser::verifyCycle() {

        foreach(ElementMap::value_type & el, elements) {
          el.second->finished = false;
          el.second->visited = false;
        }

        foreach(ElementMap::value_type & el, elements) {

          LOGDEBUG(&el << " Element  id:" << el.first << " have childs:" << el.second->getChilds().size());
          if (el.second->getChilds().size() > 0) {
            LOGDEBUG("and childchild:" << (*el.second->getChilds().begin())->getChilds().size());
          }
          verifyCycle(el.second);
        }
      }

      void GraphParser::verifyCycle(Ptr<Graph::Element> element) {

        LOGDEBUG(element << " inner verify Element id:" << element->id << " have childs:" << element->getChilds().size());

        foreach(Ptr<Graph::Element> el, element->getChilds()) {
          verifyCycle(el);
        }
        /*if(element.finished)
          return;
        if(element.visited)
          throw GraphException(std::string("cyclic redundancy found:"));
        element.visited=true;
        foreach(Graph::Element & el, element.linksTo) {
          verifyCycle(el);
        }
        element.finished=true;*/
      }

      void rec(Ptr<Graph::Element> el) {
        LOGDEBUG("Element:" << el->id << " childs:" << el->getChilds().size() << " parents: " << el->getParents().size());
        std::list<Ptr<Graph::Element> >::iterator it = el->getChilds().begin();
        for (; it != el->getChilds().end(); it++) {
          rec(*it);
        }
      }

      GraphParser::ElementMap & GraphParser::getElementMap() {
        //rec((*elements.begin()).second );
        return elements;
      }
    }
  }
}
